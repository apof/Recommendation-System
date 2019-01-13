#include "HyperCube.h"

int K;

int main(int argc, char** argv){


 char* input_file;
 char* query_file;
 char* output_file;
 char* metric;
 double range;
 int M;
 int probes;


 //read command line arguments
 if(argc!=17) { 
    cout<<"Wrong arguments, must be: -d <inputfile> -q <queryfile> -k <int> -M <int> -probes <int> -o <outputfile> -m <metric> -r <radius>"<<endl; 
    return 0; 
 }
 else
 {
    input_file = new char[strlen(argv[2]) + 1];
    strcpy(input_file,argv[2]);
    query_file = new char[strlen(argv[4]) + 1];
    strcpy(query_file,argv[4]);
    output_file = new char[strlen(argv[12]) + 1];
    strcpy(output_file,argv[12]);

    K=atoi(argv[6]);
    M=atoi(argv[8]);
    probes = atoi(argv[10]);

    metric = new char[strlen(argv[14]) + 1];
    strcpy(metric,argv[14]);

    range = atof(argv[16]);

 }

 ofstream outfile(output_file);

//randomize all random numbers generations in my programme
srand(time(NULL)); 

// buffer to read every line of the input file
char buffer[12000];

// read input data
ifstream inputfile(input_file);

//create cube
Cube* cube = new Cube(probes,M,K);

// Store Vector Pointers in order to create and destroy Vectors only Once
MyVector** vec_pointers =  new MyVector*[DATA_NUMBER];
for(int i=0; i<DATA_NUMBER; i++) vec_pointers[i] = NULL;

int data_index = 0;

cout<<"Reading input and Loading data to cube..."<<endl;

// Store Vectors in LSH Tables and store their pointers in vec_pointers
while (inputfile.getline(buffer, sizeof(buffer)))
{

    string id = to_string(data_index);
    id = "ID_"+id;

    string type("int");
    vec_pointers[data_index] = new MyVector(type,id);
    vec_pointers[data_index]->intVectorInitialization(buffer);

    if(strcmp(metric,"eucledian")==0)
        cube->insert_eucledian(vec_pointers[data_index]);
    if(strcmp(metric,"cosine")==0)
        cube->insert_cosine(vec_pointers[data_index]);

    data_index++;

}

// read query data
ifstream queryfile(query_file);


int query_index = 0;

double max_approx_fraction = 0.0;
double mean_time = 0.0;

double mean_real = 0.0;

long double sum1 = 0.0;
long double sum2 = 0.0;

// Make KNN Search
while (queryfile.getline(buffer, sizeof(buffer)))
{

    string query = to_string(query_index);
    query = "QUERY_" + query;

    string type("int");
    MyVector *query_vec = new MyVector(type,query);
    query_vec->intVectorInitialization(buffer);

    outfile<<"Query: "<<query_vec->id<<endl;

    if(strcmp(metric,"eucledian")==0)
    {

    //LSH range search eucledian
    outfile<<"R-Near Neighbors(ID and Distance):"<<endl;
    cube->range_search_eucledian(query_vec,range,outfile);

    clock_t time1, time2, time3;
    double tReal,tLSH;

    time1 = clock();
    // exhaustive KNN search eucledian
    outfile<<"Nearest Neighbor and true distance: ";
    double ret1 = query_vec->exhaustive_eucledian_KNNsearch(vec_pointers,outfile);
    time2 = clock();

    //LSH NN search eucledian
    outfile<<"distance LSH: ";
    double ret2 = cube->NN_search_eucledian(query_vec);
    outfile<<ret2<<endl;
    time3 = clock();

    tReal = ((double) (time2 - time1)) / CLOCKS_PER_SEC;
    tLSH = ((double) (time3 - time2)) / CLOCKS_PER_SEC;

    double approx = (ret2)/(ret1);

    sum2 += ret2;

    sum1 += ret1;

    if(approx > max_approx_fraction)
        max_approx_fraction = approx;

    outfile<<"tReal: "<<tReal<<endl;
    outfile<<"tLSH: "<<tLSH<<endl;

    mean_time += tLSH;

    mean_real += tReal;

    outfile<<"====================="<<endl;
    }
    else if(strcmp(metric,"cosine")==0)
    { 

    outfile<<"R-Near Neighbors(ID and Distance):"<<endl;
    //LSH range search cosine
    cube->range_search_cosine(query_vec,range,outfile);

    clock_t time1, time2, time3;
    double tReal,tLSH;

    time1 = clock();
    // exhaustive KNN search cosine
    outfile<<"Nearest Neighbor and true distance: ";
    double ret1 = query_vec->exhaustive_cosine_KNNsearch(vec_pointers,outfile);
    time2 = clock();
    //LSH NN search cosine
    outfile<<"distance LSH: ";
    double ret2 = cube->NN_search_cosine(query_vec);
    outfile<<ret2<<endl;
    time3 = clock();
   

    tReal = ((double) (time2 - time1)) / CLOCKS_PER_SEC;
    tLSH = ((double) (time3 - time2)) / CLOCKS_PER_SEC;


    double approx = (ret2)/(ret1);

    sum1 += (ret1);
    sum2 += (ret2);

    if(approx > max_approx_fraction)
        max_approx_fraction = approx;

    outfile<<"tReal: "<<tReal<<endl;
    outfile<<"tLSH: "<<tLSH<<endl;

    mean_time += tLSH;

    mean_real += tReal;


    outfile<<"====================="<<endl;
    }

    query_index++;

    delete query_vec;
}

    mean_time = mean_time/(query_index);
    mean_real = mean_real/(query_index);

    outfile<<"STATS: "<<endl;
    outfile<<"Mean tLSH: "<<mean_time<<endl;
    outfile<<"Mean tReal: "<<mean_real<<endl;
    outfile<<"Max approximate fraction: "<<max_approx_fraction<<endl;
    outfile<<"Mean approximate fraction: "<<(sum2/sum1)<<endl;


//cout<<cube->get_size()<<endl;

delete cube;

//destroy the data vectors
for(int i=0; i<DATA_NUMBER; i++)
    if(vec_pointers[i]!=NULL) { delete vec_pointers[i]; vec_pointers[i] = NULL; }
    delete []vec_pointers;

inputfile.close();
outfile.close();
queryfile.close();

delete []input_file;
delete []query_file;
delete []output_file;
delete []metric;

return 0;

}
#include "../../Part1/Proj1_updated/HashTable.h"
#include "../../Part1/Proj1_updated/HyperCube.h"
#include "cluster.h"

int K;

int main(int argc, char** argv){

 char* input_file;
 char* config_file;
 char* output_file;
 char* metric;
 int l;
 int cluster_num;
 int initialization;
 int update;
 int assignment;
 int complete;

 if(argc!=9 && argc!=10) { 
    cout<<"Wrong arguments, must be: -d <inputfile> -c <configuration_file> -o <outputfile> -d <metric> (-complete)"<<endl; 
    return 0; 
 }
 else
 {
    input_file = new char[strlen(argv[2]) + 1];
    strcpy(input_file,argv[2]);
    config_file = new char[strlen(argv[4]) + 1];
    strcpy(config_file,argv[4]);
    output_file = new char[strlen(argv[6]) + 1];
    strcpy(output_file,argv[6]);

    metric = new char[strlen(argv[8]) + 1];
    strcpy(metric,argv[8]);
 }

 if(argc==10)
 	complete = 1;
 else
 	complete = 0;


ofstream outfile(output_file);

srand(time(NULL)); 

char buffer[12000];

ifstream inputfile(input_file);
ifstream configfile(config_file);

K=4;
l=5;

while (configfile.getline(buffer, sizeof(buffer)))
{
    char* pch = strtok(buffer," ");
    if(strcmp(pch,"number_of_clusters:")==0)
   	{
   		pch = strtok(NULL," ");
   		cluster_num = atoi(pch);
   	}
   	else if(strcmp(pch,"number_of_hashtables:")==0)
    {
    	pch = strtok(NULL," ");
   		l = atoi(pch);
    }
    else if(strcmp(pch,"number_of_hashfunctions:")==0)
    {
    	pch = strtok(NULL," ");
   		K = atoi(pch);
    }
    else if (strcmp(pch,"assignment:")==0)
    {
    	pch = strtok(NULL," ");
   		assignment = atoi(pch);
    }
    else if (strcmp(pch,"initialization:")==0)
    {
    	pch = strtok(NULL," ");
   		initialization = atoi(pch);
    }
    else if (strcmp(pch,"update:")==0)
    {
    	pch = strtok(NULL," ");
   		update = atoi(pch);
    }

}

// Store Vector Pointers in order to create and destroy Vectors only Once
MyVector** vec_pointers =  new MyVector*[DATA_NUMBER];
for(int i=0; i<DATA_NUMBER; i++) vec_pointers[i] = NULL;


int data_index = 0;

Cluster_Table* Ctable = new Cluster_Table(cluster_num,metric,initialization,assignment,update,l,complete);

while (inputfile.getline(buffer, sizeof(buffer)))
{

    string id = to_string(data_index);
    id = "ID"+id;
    string type("double");

    vec_pointers[data_index] = new MyVector(type,id);
    vec_pointers[data_index]->intVectorInitialization(buffer);

    data_index++;

}

clock_t start,end;
start = clock();
Ctable->clustering(vec_pointers);
end = clock();
long double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
Ctable->compute_silhouette(outfile,cpu_time_used);

//destroy the data vectors
for(int i=0; i<DATA_NUMBER; i++)
    if(vec_pointers[i]!=NULL) { delete vec_pointers[i]; vec_pointers[i] = NULL; }
    delete []vec_pointers;

delete Ctable;

inputfile.close();
configfile.close();
outfile.close();

delete []input_file;
delete []config_file;
delete []output_file;
delete []metric;

return 0;

}

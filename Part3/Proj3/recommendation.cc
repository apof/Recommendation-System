#include "tweets_preproc.h"
#include "../../Part1/Proj1_updated/HashTable.h"
#include "../../Part2/Proj2_updated/cluster.h"
int K;

int main(int argc, char** argv){

srand(time(NULL));

string input_file;
string output_file;
string config_file = "../Datasets/config_file";
char* metric;
int l;
int cluster_num;
int initialization;
int update;
int assignment;
int complete;


// if execution is ./recommendation -create the create dataset
if(argc==2)
{
	string tweet_file = "../Datasets/tweets_dataset_big.csv";
	string coin_file = "../Datasets/coins_queries.csv";
	string lex_file = "../Datasets/vader_lexicon.csv";
	string output_file = "../Datasets/my_dataset.csv";
	string output_file_norm = "../Datasets/my_dataset_normalized.csv";


	data_preprocessing(tweet_file,coin_file,lex_file,output_file,output_file_norm);
}
else if(argc==5){

	input_file = argv[2];
	output_file = argv[4];

	cout<<input_file<<endl;
	cout<<output_file<<endl;


char buffer[12000];
ofstream outfile(output_file);
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
    else if (strcmp(pch,"metric:")==0)
    {
    	pch = strtok(NULL," ");
   		metric = new char[strlen(pch) + 1];
   		strcpy(pch,metric);
    }


}

MyVector** vec_pointers =  new MyVector*[DATA_NUMBER];
for(int i=0; i<DATA_NUMBER; i++) vec_pointers[i] = NULL;


int data_index = -1;

string* coin_array = new string[COIN_NUMBER];

while (inputfile.getline(buffer, sizeof(buffer)))
{

	if(data_index == -1)
	{

		int coin_index = 0;
		char* pch = strtok(buffer,"\t");
		pch = strtok(NULL,"\t");
		while(pch!=NULL){

			string str(pch);
			coin_array[coin_index] = str;
			coin_index++;
			pch = strtok(NULL,"\t");
		}

	}
	else
	{

    string id = to_string(data_index);
    id = "ID"+id;
    string type("double");

    vec_pointers[data_index] = new MyVector(type,id);

    vec_pointers[data_index]->intVectorInitialization(buffer);

	}

   data_index++;

}

//for(int i=0; i<DATA_NUMBER; i++)
//vec_pointers[i]->PrintVector();

complete = 0;

Cluster_Table* Ctable = new Cluster_Table(cluster_num,metric,initialization,assignment,update,l,complete);

clock_t start,end;
start = clock();
Ctable->clustering(vec_pointers);
end = clock();
long double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
Ctable->compute_silhouette(outfile,cpu_time_used);

for(int i=0; i<DATA_NUMBER; i++)
    if(vec_pointers[i]!=NULL) { delete vec_pointers[i]; vec_pointers[i] = NULL; }
    delete []vec_pointers;

inputfile.close();
configfile.close();
outfile.close();

}

return 0;
}
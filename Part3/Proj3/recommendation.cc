#include "tweets_preproc.h"
#include "../../Part1/Proj1_updated/HashTable.h"
#include "../../Part2/Proj2_updated/cluster.h"
int K;

int main(int argc, char** argv){

srand(time(NULL));

string vader_lexicon;
string tweet_dataset;
string coin_lexicon;

string input_file;
string input_flags;
string output_file;
string config_file;
char* metric;
int l;
int cluster_num;
int initialization;
int update;
int assignment;
int complete;

int Problem;
int Recommendation;


// if execution is ./recommendation -create the create dataset
if(argc!=3 && argc!=4 && argc!= 5)
  cout<<"Wrong arguments!"<<endl;
else
{
  config_file = argv[2];
  ifstream configfile(config_file);

  // default k,l values
  K=4;
  l=5;

  char buffer[12000];

  while (configfile.getline(buffer, sizeof(buffer)))
  {
    char* pch = strtok(buffer," ");
    if(strcmp(pch,"number_of_clusters:")==0)
    {
      pch = strtok(NULL," ");
      cluster_num = atoi(pch);
    }
    if(strcmp(pch,"Problem:")==0)
    {
      pch = strtok(NULL," ");
      Problem = atoi(pch);
    }
    if(strcmp(pch,"Recommendation:")==0)
    {
      pch = strtok(NULL," ");
      Recommendation = atoi(pch);
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
      strcpy(metric,pch);
    }
    else if (strcmp(pch,"vader_lexicon:")==0)
    {
      pch = strtok(NULL," ");
      char* vd = new char[strlen(pch) + 1];
      strcpy(vd,pch);
      string str(vd);
      vader_lexicon = str;
    }
    else if (strcmp(pch,"coin_lexicon:")==0)
    {
      pch = strtok(NULL," ");
      char* vd = new char[strlen(pch) + 1];
      strcpy(vd,pch);
      string str(vd);
      coin_lexicon = str;
    }
    else if (strcmp(pch,"tweet_dataset:")==0)
    {
      pch = strtok(NULL," ");
      char* vd = new char[strlen(pch) + 1];
      strcpy(vd,pch);
      string str(vd);
      tweet_dataset = str;
    }
    else if (strcmp(pch,"input_file:")==0)
    {
      pch = strtok(NULL," ");
      char* vd = new char[strlen(pch) + 1];
      strcpy(vd,pch);
      string str(vd);
      input_file = str;
    }
    else if (strcmp(pch,"output_file:")==0)
    {
      pch = strtok(NULL," ");
      char* vd = new char[strlen(pch) + 1];
      strcpy(vd,pch);
      string str(vd);
      output_file = str;
    }
    else if (strcmp(pch,"input_flags:")==0)
    {
      pch = strtok(NULL," ");
      char* vd = new char[strlen(pch) + 1];
      strcpy(vd,pch);
      string str(vd);
      input_flags = str;
    }


  }

  if((argc==4 && (strcmp(argv[3],"-create")==0)) || (argc==5))
  {
    cout<<"Creating Dataset.."<<endl;
    data_preprocessing(tweet_dataset,coin_lexicon,vader_lexicon,input_file,input_flags);
  }

  print_config(vader_lexicon,tweet_dataset,coin_lexicon,input_file,input_flags,output_file,config_file,metric,l,cluster_num,initialization,update,assignment,complete,Problem,Recommendation);

  ofstream outfile(output_file);
  ifstream inputfile(input_file);
  ifstream flagsfile(input_flags);
  
  MyVector** vec_pointers =  new MyVector*[DATA_NUMBER];
  for(int i=0; i<DATA_NUMBER; i++)
    vec_pointers[i] = NULL;


  int data_index = -1;

  string* coin_array = new string[COIN_NUMBER];

  while (inputfile.getline(buffer, sizeof(buffer)))
  {

	 if(data_index == -1)
	 {

    int coin_index = 0;
		char* pch = strtok(buffer,"\t");
    pch = strtok(NULL,"\t");
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

  data_index = 0;

  while (flagsfile.getline(buffer, sizeof(buffer)))
  {
    int* flag = new int[COIN_NUMBER];
    int flag_index = 0;

    char* pch = strtok(buffer,"\t");
    while(pch!=NULL)
    {
      flag[flag_index] = atoi(pch);
      flag_index++;
      pch = strtok(NULL,"\t");

    }

    vec_pointers[data_index]->set_flag(flag);

    data_index++;

  }



for(int i=0; i<DATA_NUMBER; i++)
  vec_pointers[i]->PrintVector();
for(int i=0; i<COIN_NUMBER; i++)
  cout<<coin_array[i]<<endl;

/*
if(Problem==1 && Recommendation==1)
{

  LSH_Tables* Tables = new LSH_Tables(l);

  for(int i=0; i<DATA_NUMBER; i++)
  {
    Tables->insert_cosine(vec_pointers[i]);
  }

  for(int i=0; i<DATA_NUMBER; i++)
  {
  list<MyVector*> l = Tables->range_search_cosine(vec_pointers[i],0.0005);
  cout<<l.size()<<endl;
  }

}
*/
  /*
  Cluster_Table* Ctable = new Cluster_Table(cluster_num,metric,initialization,assignment,update,l,complete);

  clock_t start,end;
  start = clock();
  Ctable->clustering(vec_pointers);
  end = clock();
  long double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  Ctable->compute_silhouette(outfile,cpu_time_used);

 */

  for(int i=0; i<DATA_NUMBER; i++)
    if(vec_pointers[i]!=NULL) { delete vec_pointers[i]; vec_pointers[i] = NULL; }
    delete []vec_pointers;

  inputfile.close();
  configfile.close();
  outfile.close();
}

return 0;
}
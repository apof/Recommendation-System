#include "tweets_preproc.h"
#include "recomm_methods.h"

int K;

int main(int argc, char** argv){

srand(time(NULL));


  // default k,l values
  K=4;
  int l=5;
  int cluster_num;
  int Recommendation;

  string config_file = argv[8];

  ifstream configfile(config_file);

  char buffer[12000];


  while (configfile.getline(buffer, sizeof(buffer)))
  {
  char* pch = strtok(buffer," ");

  if(strcmp(pch,"Recommendation:")==0)
  {
    pch = strtok(NULL," ");
    Recommendation = atoi(pch);
  }
  else if(strcmp(pch,"cluster_num:")==0)
  {
    pch = strtok(NULL," ");
    cluster_num = atoi(pch);
  }
  }

  //cout<<Recommendation<<" "<<cluster_num<<endl;

  string input_file = argv[2];
  string input_flags = argv[4];
  string output_file = argv[6];

  ifstream inputfile(input_file);
  ifstream flagsfile(input_flags);
  
  MyVector** vec_pointers =  new MyVector*[TOTAL_DATA_NUMBER];
  for(int i=0; i<TOTAL_DATA_NUMBER; i++)
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

/*for(int i=0; i<DATA_NUMBER; i++)
  vec_pointers[i]->PrintVector();
for(int i=0; i<COIN_NUMBER; i++)
  cout<<coin_array[i]<<endl;
*/
  
if(Recommendation==1)
{
  string str = "cosine";
  char *cstr = new char[str.length() + 1];
  strcpy(cstr, str.c_str());

  if(argc!=10)
    recommendation_based_on_lsh(vec_pointers,l,cstr,coin_array,output_file);
  else
    validation_on_lsh(vec_pointers,l,cstr);

  delete []cstr;
}
else if(Recommendation==2)
{
  string str = "eucledian";
  char *cstr = new char[str.length() + 1];
  strcpy(cstr, str.c_str());

  if(argc!=10)
    recommendation_based_on_clustering(vec_pointers,l,cstr,coin_array,output_file,cluster_num,1,1,1);
  else
    validation_on_clustering_new(vec_pointers,l,cstr,cluster_num,1,1,1);

  delete []cstr;

}

  
  for(int i=0; i<TOTAL_DATA_NUMBER; i++)
    if(vec_pointers[i]!=NULL) { delete vec_pointers[i]; vec_pointers[i] = NULL; }
    delete []vec_pointers;


  inputfile.close();
  flagsfile.close();
  configfile.close();

return 0;
}
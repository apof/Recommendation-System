#include "tweets_preproc.h"

int K;

int main(int argc, char** argv){

srand(time(NULL));

string config_file = argv[1];

string vader_lexicon;
string tweet_dataset;
string coin_lexicon;
string input_file;
string input_flags;
string clustering_file;

char buffer[12000];

ifstream configfile(config_file);

while (configfile.getline(buffer, sizeof(buffer)))
{
	char* pch = strtok(buffer," ");

	if(strcmp(pch,"vader_lexicon:")==0)
	{
		pch = strtok(NULL," ");
		string str(pch);
		vader_lexicon = str;
	}
	else if(strcmp(pch,"tweet_dataset:")==0)
	{
		pch = strtok(NULL," ");
		string str(pch);
		tweet_dataset = str;
	}
	else if(strcmp(pch,"coin_lexicon:")==0)
	{
		pch = strtok(NULL," ");
		string str(pch);
		coin_lexicon = str;
	}
	else if(strcmp(pch,"input_file:")==0)
	{
		pch = strtok(NULL," ");
		string str(pch);
		input_file = str;
	}
	else if(strcmp(pch,"input_flags:")==0)
	{
		pch = strtok(NULL," ");
		string str(pch);
		input_flags = str;
	}
	else if(strcmp(pch,"clustering_file:")==0)
	{
		pch = strtok(NULL," ");
		string str(pch);
		clustering_file = str;
	}
}

/*cout<<vader_lexicon<<endl;
cout<<coin_lexicon<<endl;
cout<<tweet_dataset<<endl;
cout<<clustering_file<<endl;
cout<<input_flags<<endl;
cout<<input_file<<endl;*/

cout<<"Creating Dataset.."<<endl;
data_preprocessing(tweet_dataset,coin_lexicon,vader_lexicon,input_file,input_flags,clustering_file);

}
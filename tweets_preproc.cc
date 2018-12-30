#include "tweets_preproc.h"

Tweet** read_tweets(string file){

	ifstream inputfile(file);

	int data_index  = 0;

	char buffer[12000];

	Tweet** tweet_pointers =  new Tweet*[TWEET_NUMBER];
	for(int i=0; i<TWEET_NUMBER; i++) tweet_pointers[i] = NULL;

	while (inputfile.getline(buffer, sizeof(buffer)))
	{
		char* pch = strtok(buffer,"\t");
		int uid = atoi(pch);
		pch = strtok(NULL,"\t");
		int tid = atoi(pch);

		list<string> l;
		while(pch!=NULL)
		{
			pch = strtok(NULL,"\t\n");
			if(pch!=NULL)
			{
				string str(pch);
				l.push_back(str);
			}
		}

		tweet_pointers[data_index] = new Tweet(tid,uid,l);

		data_index++;
	}

	return tweet_pointers;
}

Coin** read_coins(string file){

	int coin_index  = 0;

	char buffer[12000];

	ifstream lex1file(file);

	Coin** coin_pointers =  new Coin*[COIN_NUMBER];
	for(int i=0; i<COIN_NUMBER; i++) coin_pointers[i] = NULL;

	while (lex1file.getline(buffer, sizeof(buffer)))
	{

		char* pch = strtok(buffer,"\t");
		string name(pch);

		list<string> l;
		while(pch!=NULL)
		{
			pch = strtok(NULL,"\t\n");
			if(pch!=NULL)
			{
				string str(pch);
				l.push_back(str);
			}
		}

		coin_pointers[coin_index] = new Coin(name,l);

		coin_index++;
	}

	return coin_pointers;


}

unordered_map<string, double> read_lexicon(string file){

	ifstream lex2file(file);

	char buffer[12000];

	unordered_map<string, double> lexicon;

	while (lex2file.getline(buffer, sizeof(buffer)))
	{
		char* pch = strtok(buffer,"\t");
		string name(pch);

		pch = strtok(NULL,"\t");
		double val = atof(pch);

		lexicon[name] = val;

	}

	return lexicon;
}

unordered_map<string,int> convert_coins_to_lexicon(Coin** C){

	unordered_map<string,int> coin_lex; 

	for(int i=0; i<COIN_NUMBER; i++)
	{
		list<string> l = C[i]->get_list();

		for (auto n : l)
			coin_lex[n] = i;
	}

	return coin_lex;
}

void data_preprocessing(string file1,string file2,string file3){

	Tweet** T = read_tweets(file1);

	Coin** C = read_coins(file2);
	unordered_map<string,double> lexicon = read_lexicon(file3);

	for(int i=0; i<TWEET_NUMBER; i++){
		T[i]->print_tweet();
	}


	for(int i=0; i<COIN_NUMBER; i++){
		C[i]->print_coin();
	}

	for (auto x : lexicon) 
      cout << x.first << " " << x.second << endl;

  	unordered_map<string,int> coin_lex = convert_coins_to_lexicon(C);

  	for (auto x : coin_lex) 
      cout << x.first << " " << x.second << endl;



}
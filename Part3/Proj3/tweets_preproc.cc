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
			pch = strtok(NULL,"\t");
			if(pch!=NULL)
			{
				string str(pch);
				l.push_back(str);
			}
		}

		// erase newline character from the last string of the tweet and put it back to the list
		string back = l.back();
		l.pop_back();
		back.erase( back.end()-1 );
		l.push_back(back);

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
			pch = strtok(NULL,"\t");
			if(pch!=NULL)
			{
				string str(pch);
				l.push_back(str);
			}
		}

		// erase newline character from the last string of the coins list and put it back to the list
		string back = l.back();
		l.pop_back();
		back.erase( back.end()-1 );
		l.push_back(back);

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

		coin_lex[C[i]->name] = i;
	}

	return coin_lex;
}

void stand_out_info_coins(Tweet** T,unordered_map<string,double> lexicon,unordered_map<string,int> coin_lex){

	for(int i=0; i<TWEET_NUMBER; i++)
	{
		for (auto x : T[i]->tweet)
		{

			unordered_map<string,int>::iterator it;
			it = coin_lex.find(x);

			if (it != coin_lex.end())
			{
				T[i]->coins.push_back(x);
			}
			else 
			{				 
				T[i]->info.push_back(x);
				unordered_map<string,double>::iterator itt;

				itt = lexicon.find(x);

				if (itt != lexicon.end())
				{
					T[i]->value += itt->second;
					T[i]->words_found++;
				}
				else
				{
					//cout<<"Not found in lexicon "<<x<<endl;
				}

			}
		}
		int alpha = 15;
		if(T[i]->value!=0)
		{
			T[i]->value = T[i]->value/sqrt((T[i]->value)*(T[i]->value)+alpha);
		}
	}
}

User** create_vectors(Tweet** T,int user_number,unordered_map<string,int> coin_lex){

	User** user_pointers =  new User*[user_number];
	for(int i=0; i<user_number; i++) user_pointers[i] = new User(i+1);

	for(int i=0; i<TWEET_NUMBER; i++)
	{
		user_pointers[(T[i]->user_id)-1]->user_tweets.push_back(T[i]);
	}

	for(int i=0; i<user_number; i++)
	{

		for (auto t : user_pointers[i]->user_tweets)
		{

			for(auto c: t->coins)
			{

				unordered_map<string,int>::iterator it;
				it = coin_lex.find(c);

				user_pointers[i]->flag_vector[it->second] = 1;
				user_pointers[i]->vector[it->second] += t->value; 

			}

		}

	}

	/*for(int i=0; i<user_number; i++)
	{
		user_pointers[i]->print_user();
	}*/

	return user_pointers;

}

void data_preprocessing(string file1,string file2,string file3,string out){

	Tweet** T = read_tweets(file1);
	Coin** C = read_coins(file2);
	unordered_map<string,double> lexicon = read_lexicon(file3);

	/*for(int i=0; i<COIN_NUMBER; i++){
		C[i]->print_coin();
	}

	for (auto x : lexicon) 
      cout << x.first << " " << x.second << endl;
	*/
  	unordered_map<string,int> coin_lex = convert_coins_to_lexicon(C);

  	/*for (auto x : coin_lex) 
      cout << x.first <<" "<< x.second << endl;
	
	*/
	stand_out_info_coins(T,lexicon,coin_lex);

	int user_number = T[TWEET_NUMBER-1]->user_id;

	/*for(int i=0; i<TWEET_NUMBER; i++){
		T[i]->print_tweet();
	}*/

	User** users = create_vectors(T,user_number,coin_lex);

	ofstream outfile(out);

	outfile<<" "<<"\t";
	for(int i=0; i<COIN_NUMBER; i++)
	{
		outfile<<C[i]->name<<"\t";
	}
	outfile<<endl;
	for(int i=0; i<user_number; i++)
	{
		outfile<<users[i]->id<<"\t";
		for(int k=0; k<COIN_NUMBER; k++)
		{
			outfile<<users[i]->vector[k]<<"\t";
		}
		outfile<<endl;
	}

}
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

void data_normalize(User** user,int user_number){

	for(int i=0; i<user_number; i++)
	{
		user[i]->normalize();
	}

}

User** create_iconic_users(User** users,int user_number,string clustering_file){

	ifstream inputfile(clustering_file);

	MyVector** vec_pointers =  new MyVector*[CLUSTERING_FILE_SIZE];
  	for(int i=0; i<CLUSTERING_FILE_SIZE; i++)
    	vec_pointers[i] = NULL;

	int data_index = 0;

	char buffer[15000];

  	while (inputfile.getline(buffer, sizeof(buffer)))
  	{

  		string id = to_string(data_index);
    	id = "ID"+id;
    	string type("double");

    	vec_pointers[data_index] = new MyVector(type,id);

    	char dest[10000] = "0.00\t";
    	strcat(dest,buffer);

    	vec_pointers[data_index]->intVectorInitialization(dest);

    	data_index++;

	 }


	User** ic_users = new User*[ICONIC_USERS];
	for(int i=0; i<ICONIC_USERS; i++)
		ic_users[i] = new User(i+1);

	cout<<"Clustering to create iconic users.."<<endl;

	Cluster_Table* Ctable = new Cluster_Table(ICONIC_USERS,"cosine",1,1,1,5,0);

	int flag = 1;

	Cluster** clusters;


	while(flag!=0)
	{
  	Ctable->clustering(vec_pointers);
  	clusters = Ctable->get_clusters();

  	int fl = 1;
  	for(int p=0; p<ICONIC_USERS; p++)
  	{
  		if(clusters[p]->get_list().size()==0)
  			fl =0;

  	}

  	if(fl==1)
  		flag=0;
  	else
  		cout<<"Zero cluster-Repeating clustering!"<<endl;

  	}

  	clusters = Ctable->get_clusters();


  	for(int i=0; i<ICONIC_USERS; i++)
  	{
  		for(auto elem : clusters[i]->get_list())
  		{
  			
  				for(int k=0; k<user_number; k++)
  				{

  					if(users[k]->id == elem.Vector->int_id)
  					{
  						for(int j=0; j<COIN_NUMBER; j++)
  						{
  							if(users[k]->flag_vector[j]==1)
  							{
  								ic_users[i]->flag_vector[j]=1;
  								ic_users[i]->vector[j] += users[k]->vector[j];
  							}

  						}

  					}
  				}
  			
  		}
  	}

  	for(int i=0; i<user_number; i++)
  		delete users[i];
  	delete users;

  	return ic_users;

}

void data_preprocessing(string file1,string file2,string file3,string out_norm,string input_flags,string clustering_file){

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

	if(ICONIC_MODE==1){

		users = create_iconic_users(users,user_number,clustering_file);
		user_number = ICONIC_USERS;
	}

	data_normalize(users,user_number);

	ofstream outfile(out_norm);
	ofstream outfile2(input_flags);

	// write user vectors into a csv file
	outfile<<"mean_val"<<"\t";
	outfile<<"user_id"<<"\t";
	for(int i=0; i<COIN_NUMBER; i++)
	{
		outfile<<C[i]->name<<"\t";
	}
	outfile<<endl;
	for(int i=0; i<user_number; i++)
	{
		if(users[i]->flag==1)
		{
		outfile<<users[i]->mean_value<<"\t";
		outfile<<users[i]->id<<"\t";
		for(int k=0; k<COIN_NUMBER; k++)
		{
			outfile2<<users[i]->flag_vector[k]<<"\t";
			outfile<<users[i]->vector[k]<<"\t";
		}
		outfile<<endl;
		outfile2<<endl;
		}
	}

	for(int i=0; i<TWEET_NUMBER; i++)
		delete T[i];
	delete []T;

	for(int i=0; i<COIN_NUMBER; i++)
		delete C[i];
	delete []C;

	for(int i=0; i<user_number; i++)
		delete users[i];
	delete []users;


}
#ifndef TWEETS_PREPROC
#define TWEETS_PREPROC

#include "../../Part1/Proj1_updated/Vector.h"

class Tweet {

public:

	int tweet_id;
	int user_id;
	list <string> tweet;
	double value;
	list<string> coins;
	list<string> info;
	int words_found;

	Tweet(int t_id, int u_id, list<string> l){
		tweet_id = t_id;
		user_id = u_id;
		tweet = l;
		value = 0.0;
		words_found = 0;
	}

	void print_tweet(){

		cout<<tweet_id<<"-"<<user_id<<"-"<<value<<" "<<words_found<<endl;
		cout<<"Tweet: ";
		for (auto n : tweet)
			cout<<n<<" ";
		cout<<endl;
		cout<<"Tweets's coins: ";
		for (auto n : coins)
			cout<<n<<" ";
		cout<<endl;
		cout<<"Tweet's info: ";
		for (auto n : info)
			cout<<n<<" ";
		cout<<endl;
		cout<<"--------------------"<<endl;
		
	}


};

class Coin{

public:

	string name;
	list<string> names;

	Coin(string n,list<string> nn){

		name = n;
		names = nn;
	}

	void print_coin(){

		cout<<name<<endl;
		for (auto n : names)
			cout<<n<<" ";
		cout<<endl;

	}

	string get_name() {

		return name;
	}

	list<string> get_list(){

		return names;
	}

};


class User {

public:

	int id;
	list<Tweet*> user_tweets;
	long double* vector;
	int* flag_vector;

	User(int Id){
		id = Id;
		vector = new long double[COIN_NUMBER];
		flag_vector = new int[COIN_NUMBER];
		for(int i=0; i<COIN_NUMBER; i++)
		{
			vector[i] = 0.0;
			flag_vector[i] = 0;
		}
	}

	~User(){

			delete []vector;
			delete []flag_vector;
	
	}


	void print_user(){
		cout<<"@_@_@_@_@--->User "<<id<<" info"<<endl;
		for (auto n : user_tweets)
			n->print_tweet();
		cout<<"User Vector: ";
		for(int i=0; i<COIN_NUMBER; i++)
		{
			cout<<vector[i]<<" ";
		}
		cout<<endl;
		cout<<"User Total Coins: ";
		for(int i=0; i<COIN_NUMBER; i++)
		{
			cout<<flag_vector[i]<<" ";
		}
		cout<<endl;

	}

};

Tweet** read_tweets(string);
Coin** read_coins(string);
unordered_map<string, double> read_lexicon(string);
void data_preprocessing(string,string,string,string);
unordered_map<string,int> convert_coins_to_lexicon(Coin**);
void stand_out_info_coins(Tweet**);
User** create_vectors(Tweet**,int,unordered_map<string,int>);

#endif
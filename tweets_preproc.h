#ifndef TWEETS_PREPROC
#define TWEETS_PREPROC

#include "../../Part1/Proj1_updated/Vector.h"

class Tweet {

private:

	int tweet_id;
	int user_id;
	list <string> tweet;

public:

	Tweet(int t_id, int u_id, list<string> l){
		tweet_id = t_id;
		user_id = u_id;
		tweet = l;
	}

	void print_tweet(){

		cout<<tweet_id<<"-"<<user_id<<endl;
		for (auto n : tweet)
			cout<<n<<" ";
		cout<<endl;
	}
};

class Coin{

private:

	string name;
	list<string> names;

public:

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

Tweet** read_tweets(string);
Coin** read_coins(string);
unordered_map<string, double> read_lexicon(string);
void data_preprocessing(string,string,string);
unordered_map<string,int> convert_coins_to_lexicon(Coin**);

#endif
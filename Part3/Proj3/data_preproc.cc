#include "tweets_preproc.h"

int K;

int main(int argc, char** argv){

srand(time(NULL));

string vader_lexicon = "../Datasets_and_Configs/Given_Datasets/vader_lexicon.csv";
string tweet_dataset = "../Datasets_and_Configs/Given_Datasets/tweets_dataset_big.csv";
string coin_lexicon = "../Datasets_and_Configs/Given_Datasets/coins_queries.csv";

string input_file = "../Datasets_and_Configs/My_Datasets/my_dataset.csv";
string input_flags = "../Datasets_and_Configs/My_Datasets/my_dataset_flags.csv";
string clustering_file = "../Datasets_and_Configs/Given_Datasets/twitter_dataset_small_v2.csv";

cout<<"Creating Dataset.."<<endl;
data_preprocessing(tweet_dataset,coin_lexicon,vader_lexicon,input_file,input_flags,clustering_file);

}
#include "tweets_preproc.h"
int K;

int main(int argc, char** argv){

srand(time(NULL)); 

string tweet_file = "../Datasets/tweets_dataset_small.csv";
string coin_file = "../Datasets/coins_queries.csv";
string lex_file = "../Datasets/vader_lexicon.csv";

data_preprocessing(tweet_file,coin_file,lex_file);

return 0;
}
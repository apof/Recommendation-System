#ifndef UTILS_INCLUDED
#define UTILS_INCLUDED


#include <iostream>
#include <cstdlib>
#include <random>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <cstring>
#include <limits>
#include <list>
#include <unordered_map>
#include <cmath>
#include <algorithm>


#define DATA_VECTOR_SIZE 100
#define w 350
#define COIN_NUMBER 100
#define DATA_NUMBER 34192
#define TWEET_NUMBER 161972
#define MAX_R_RANGE 800
#define MAX_ITERS 50
#define PN 20
#define USERS_TO_RECOMM 20

extern int K;

using namespace std;
 
double marsaglia_method(double,double);
double dot_product(long double*,long double*,size_t);
long long int compute_M();
int compute_cube_size(int);
int* create_random_r();
int ckeck_same_g(int*,int*);
int convert_bits_to_int(int*);
long double cosine_similarity(long double*, long double*);
int hammingDistance(int,int);
int* next_probe(int&,int,int*);
void int_to_bin_digit(unsigned int, int,int*);
int same_v(long double* , long double*);
void print_config(string,string,string,string,string,string,string,char*,int,int,int,int,int,int,int,int);

#endif

#ifndef UTILS_INCLUDED
#define UTILS_INCLUDED


#include <iostream>
#include <cstdlib>
#include <random>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <cstring>

#define DATA_VECTOR_SIZE 129 
#define w 350
#define DATA_NUMBER 10000
#define MAX_R_RANGE 800

extern int K;

using namespace std;
 
double marsaglia_method(double,double);
double dot_product(int*,double*,size_t);
long long int compute_M();
int compute_cube_size(int);
int* create_random_r();
int ckeck_same_g(int*,int*);
int convert_bits_to_int(int*);
double cosine_similarity(int*, int*);
int hammingDistance(int,int);
int* next_probe(int&,int,int*);
void int_to_bin_digit(unsigned int, int,int*);

#endif

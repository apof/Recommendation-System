#ifndef VECTOR_INCLUDED
#define VECTOR_INCLUDED

#include "utils.h"

class MyVector {

public:

	int* intVecArray;
	long double* doubleVecArray;
	int vector_dimension;
	string id;

	MyVector(string,string);
	~MyVector();
	void intVectorInitialization(char[]);
	void GaussianVectorInitialization();
	void PrintVector();
	int* compute_eucledian_g(int*,MyVector**);
	int compute_eucledian_h(long double*,int,MyVector*);
	int compute_eucledian_f(int*,int*,long long,int);
	long double eucledian_distance(MyVector*);
	double exhaustive_eucledian_KNNsearch(MyVector**,ofstream&);
	double exhaustive_cosine_KNNsearch(MyVector**,ofstream&);
	int compute_cosine_index(MyVector**);
	int* compute_cube_f(int*);

	//new funcs for clustering

	void set_vector(long double*);
	long double compute_distance(MyVector*,char*);

};

#endif
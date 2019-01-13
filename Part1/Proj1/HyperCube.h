#ifndef HYPERCUBE_INCLUDED
#define HYPERCUBE_INCLUDED

#include "VectorList.h"

class Cube {


private:

	VectorList** Table;
	int TableSize;
	int probes;
	int M;

	int* t;
	int *r;
	MyVector **v;

public:

	Cube(int p,int m,int K){

		TableSize = compute_cube_size(K);
		probes = p;
		M = m;

		Table = new VectorList*[TableSize];
		for(int i = 0; i<TableSize; i++)
			Table[i] = new VectorList();


		t = new int[K];
		for(int i=0; i<K; i++) t[i] = (rand()/(RAND_MAX+1.0))*(w - 1);

		r = create_random_r();

  		string type("double");
		v = new MyVector*[K];
		for(int i=0; i<K; i++)
		{
			v[i] = new MyVector(type,"gaussian_vector");
			v[i]->GaussianVectorInitialization();
		}

	}

	~Cube(){

		for(int i=0; i<TableSize; i++)
			{
				delete Table[i];
				Table[i] = NULL;
			}

		delete []Table;


		for(int i=0; i<K; i++)
		{
			delete v[i];
		}

		delete []v;

		delete []r;
		delete []t;

	}

	int get_size(){

		int size = 0;

		size += sizeof(int);
		size += sizeof(int*);
		size += sizeof(int*);
		size += sizeof(MyVector**);	
		size += K*sizeof(int);	
		size += K*sizeof(int);
		size += K*(DATA_VECTOR_SIZE*(sizeof(double)) + sizeof(int) + sizeof(int*) + sizeof(double*) ); // v table

		size += sizeof(VectorList**);
		size += sizeof(class VectorList) + DATA_NUMBER*sizeof(class ListNode);

		size += 2*sizeof(int);

		return size;

	}

	void insert_eucledian(MyVector*);
	void insert_cosine(MyVector*);
	double NN_search_cosine(MyVector*);
	double range_search_cosine(MyVector*,double, ofstream&);
	double NN_search_eucledian(MyVector*);
	void range_search_eucledian(MyVector*, double, ofstream&);
	

};

#endif
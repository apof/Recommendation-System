#ifndef HASHTABLE_INCLUDED
#define HASHTABLE_INCLUDED

#include "VectorList.h"

class HashTable{

private:

	VectorList** Table;
	int TableSize;


	///////////////
	int* t;
	int *r;
	MyVector **v;


public:

	HashTable(){

		TableSize = DATA_NUMBER/4;
		Table = new VectorList*[TableSize];
		for(int i = 0; i<TableSize; i++)
			Table[i] = new VectorList();


		/////////////////
		t = new int[K];
		for(int i=0; i<K; i++) { t[i] = (rand()/(RAND_MAX+1.0))*(w - 1); }

		r = create_random_r();

  		string type("double");
		v = new MyVector*[K];
		for(int i=0; i<K; i++)
		{
			v[i] = new MyVector(type,"gaussian_vector");
			v[i]->GaussianVectorInitialization();
		}

	}

	~HashTable(){

		for(int i=0; i<TableSize; i++)
			{
				delete Table[i];
				Table[i] = NULL;
			}

		delete []Table;


		/////
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

		return size;

	}

	int Hashtable_info(){

		int sum = 0;
		for(int i=0; i<TableSize; i++)
		{
			sum += Table[i]->get_size();
		}

		return sum;

	}

	void insert_eucledian(MyVector*);
	void insert_cosine(MyVector*);
	double NN_search_eucledian(MyVector*,int,int);
	void range_search_eucledian(MyVector*,double,ofstream&,int,int);
	double NN_search_cosine(MyVector*,int,int);
	void range_search_cosine(MyVector*,double,ofstream&,int,int);
	
    			
};


class LSH_Tables{

private:

	HashTable** Tables;
	int Num_Of_Tables;

public:

	LSH_Tables(int L){

		Num_Of_Tables = L;
		Tables = new HashTable*[Num_Of_Tables];
		for(int i=0; i<Num_Of_Tables; i++)
			Tables[i] = new HashTable();

	}

	~LSH_Tables(){

		for(int i=0; i<Num_Of_Tables; i++)
		{

			delete Tables[i];
			Tables[i] = NULL;
		}

		delete []Tables;

	}

	int get_size(){

		int sum = 0;

		int int_vector_size = (DATA_VECTOR_SIZE*(sizeof(int)) + sizeof(int) + sizeof(double*) + sizeof(int*) );

		sum += DATA_VECTOR_SIZE*int_vector_size;

		sum += sizeof(int);
		sum += sizeof(HashTable**);

		sum += Num_Of_Tables*(Tables[0]->get_size());

		return sum;

	}


	void LSH_capacity(){

		for(int i=0; i<Num_Of_Tables; i++)
		{

			cout<<"------>>"<<Tables[i]->Hashtable_info()<<endl;
		}

	}


	void insert_eucledian(MyVector*);
	void insert_cosine(MyVector*);
	double NN_search_cosine(MyVector*);
	double range_search_cosine(MyVector*,double, ofstream&);
	double NN_search_eucledian(MyVector*);
	void range_search_eucledian(MyVector*, double, ofstream&);

};


#endif
#include "Vector.h"

	MyVector::MyVector(string type, string ID) {

		if(type.compare("int")==0) 
		{ 
			intVecArray = new int[DATA_VECTOR_SIZE];
			doubleVecArray = NULL;
		}
		else if (type.compare("double")==0)
		{
			doubleVecArray = new double[DATA_VECTOR_SIZE];
			intVecArray = NULL;
		}
		vector_dimension = DATA_VECTOR_SIZE;
		id = ID;
	}

	MyVector::~MyVector() {

		if (intVecArray != NULL)
		delete []intVecArray;
		if (doubleVecArray != NULL)
		delete []doubleVecArray;
	}

	void MyVector::intVectorInitialization(char buffer[]) {

		char* pch = strtok(buffer," ");
		int index = 0;
	
    	while(pch!=NULL){
    	intVecArray[index] = atoi(pch);
    	index++;
    	pch = strtok (NULL, "\n ");
    	}

    	if (index!=DATA_VECTOR_SIZE)
    	{
    		cout<<"Different vector size: "<<index<<" "<<id<<endl;
    		intVecArray[index] = 0;
    	}

	}


	void MyVector::GaussianVectorInitialization() {

		for (int i=0; i<vector_dimension; i++)
		{
			doubleVecArray[i] = marsaglia_method(0.0,1.0);
		}
	}

	void MyVector::PrintVector(){

		if (intVecArray!=NULL)
		{
			for(int i=0; i<vector_dimension; i++)
				cout<<intVecArray[i]<<" ";
				cout<<endl;
				//cout<<"-----------------Vector dimension--------        "<< vector_dimension <<endl;
		}
		else if (doubleVecArray!=NULL)
		{
			for(int i=0; i<vector_dimension; i++)
				cout<<doubleVecArray[i]<<" ";
				cout<<endl;
				cout<<"-----------------Vector dimension--------        "<< vector_dimension <<endl;
		}

	}

	int* MyVector::compute_eucledian_g(int* t,MyVector** v){

  		int* g = new int[K];

  		for(int i=0; i<K; i++)
  		{
    		g[i]= compute_eucledian_h(intVecArray,t[i],v[i]);
  		}

  		return g;
	}


	int MyVector::compute_eucledian_h(int* vector, int t, MyVector* v){

  	int dot_prod = dot_product(vector,v->doubleVecArray,DATA_VECTOR_SIZE);

  	return floor((dot_prod + t)/w);

	}

	int MyVector::compute_eucledian_f(int* g,int* r,long long M,int TableSize){


		long long int res = 0;

		for(int i =0; i<K; i++)
		{

			res += g[i]*r[i];
		}

		res = ((res%M) + M)%M;

		res = res%TableSize;

		if( (res >= TableSize) || ( res < 0 ) )
		{
			cout<<"index = "<<res<<" overflow or underflow bug in f creation"<<endl;
			return -1;
		}

		return res;

	}

	long double MyVector::eucledian_distance(MyVector* data){

	long double Sum = 0.0;

	for(int i=0;i<DATA_VECTOR_SIZE; i++)
	{
		Sum += pow(((data->intVecArray[i])-intVecArray[i]),2.0);
	}

	return sqrt(Sum);

	}

	double MyVector::exhaustive_eucledian_KNNsearch(MyVector** vec_pointers, ofstream& outfile)
	{

		string res;
		string neighbor;

		long double min_dist = 10000000.0;
		long double dist = 0.0;
		for(int i=0; i<DATA_NUMBER; i++)
		{
			dist = eucledian_distance(vec_pointers[i]);
			if(dist<min_dist)
			{
				min_dist = dist;
				neighbor = vec_pointers[i]->id;
			}
		}


		outfile<<neighbor<<"-"<<min_dist<<endl;

		return min_dist;
		
	}

	double MyVector::exhaustive_cosine_KNNsearch(MyVector** vec_pointers, ofstream& outfile)
	{

		string res;
		string neighbor;

		double min_dist = 2;
		double dist = 0.0;
		for(int i=0; i<DATA_NUMBER; i++)
		{
			dist = cosine_similarity(vec_pointers[i]->intVecArray,intVecArray);
			if(dist < min_dist)
			{
				min_dist = dist;
				neighbor = vec_pointers[i]->id;
			}
		}

		outfile<<neighbor<<"-"<<min_dist<<endl;

		return min_dist;

	}




	int MyVector::compute_cosine_index(MyVector** v){

		int* g = new int[K];

		for(int i=0; i<K; i++)
		{

  			int dot_prod = dot_product(intVecArray,v[i]->doubleVecArray,DATA_VECTOR_SIZE);

  			if (dot_prod < 0)
  				g[i] = 0;
  			else if (dot_prod >= 0)
  				g[i] = 1;

		}

		int res = convert_bits_to_int(g);

		int TableSize = DATA_NUMBER/4;

		if( (res >= TableSize) || ( res < 0 ) )
		{
			cout<<"index = "<<res<<" overflow or underflow bug in f creation"<<endl;
			return -1;
		}

		delete []g;

		return res;

	}

	int* MyVector::compute_cube_f(int* g){

		int *indx = new int[K];

		for(int i=0; i<K; i++)
		{
			indx[i] = ((g[i]%2) + 2)%2;
		}

		delete []g;

		return indx;
	}
	

	
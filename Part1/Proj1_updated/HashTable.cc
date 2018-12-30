#include "HashTable.h"


	void LSH_Tables::insert_eucledian(MyVector* vec) {


		for(int i=0; i<Num_Of_Tables; i++)
		{

    		Tables[i]->insert_eucledian(vec);
    			
		}

	}


	void LSH_Tables::insert_cosine(MyVector* vec){

		for(int i=0; i<Num_Of_Tables; i++)
		{

    		Tables[i]->insert_cosine(vec);
    			
    	}


	}

	double LSH_Tables::NN_search_cosine(MyVector* vec){


		double dist = 0.0;
		double min_dist = 2.0;

		for(int i=0; i<Num_Of_Tables; i++)
		{
    		dist = Tables[i]->NN_search_cosine(vec,0,-1);
    		if(dist < min_dist)
    			min_dist = dist;
    	}

		return min_dist;

	}

	list<MyVector*> LSH_Tables::range_search_cosine(MyVector* vec,double range){

		list<MyVector*> range_list;

		for(int i=0; i<Num_Of_Tables; i++)
		{
    		range_list.splice(range_list.end(),Tables[i]->range_search_cosine(vec,range,0,-1));
		}

		return range_list;

	}




	double LSH_Tables::NN_search_eucledian(MyVector* vec){

		double dist = 0.0;
		double min_dist = 100000;

		for(int i=0; i<Num_Of_Tables; i++)
		{
    			dist = Tables[i]->NN_search_eucledian(vec,0,-1);
    			if(min_dist > dist)
    				min_dist = dist;
		}

		return min_dist;

	}

	list<MyVector*> LSH_Tables::range_search_eucledian(MyVector* vec, double range){


		list<MyVector*> range_list;

		for(int i=0; i<Num_Of_Tables; i++)
		{
    		range_list.splice(range_list.end(),Tables[i]->range_search_eucledian(vec,range,0,-1));
		}

		return range_list;

	}


	void HashTable::insert_eucledian(MyVector* vec){

		long long M = compute_M();

    	int* g = vec->compute_eucledian_g(t,v);
    	int index = vec->compute_eucledian_f(g,r,M,DATA_NUMBER/4);

    	if(index!=-1)
    	{
    		Table[index]->insert(vec,g);
    	}

	}


	void HashTable::insert_cosine(MyVector* vec){

		int index = vec->compute_cosine_index(v);

    	if(index!=-1)
    	{
    		Table[index]->insert(vec,NULL);
    	}
	}


	double HashTable::NN_search_eucledian(MyVector* vec,int a,int b)
	{

		long long M = compute_M();

    	int *g = vec->compute_eucledian_g(t,v);
    	int index = vec->compute_eucledian_f(g,r,M,DATA_NUMBER/4);

    	if(index!=-1)
    	{
    		return Table[index]->search_eucledian(vec,g,a,b);
    	}
    			
	}

	list<MyVector*> HashTable::range_search_eucledian(MyVector* vec,double range,int a,int b)
	{

		long long M = compute_M();

    	int* g = vec->compute_eucledian_g(t,v);
    	int index = vec->compute_eucledian_f(g,r,M,DATA_NUMBER/4);

    	if(index!=-1)
    	{
    		return Table[index]->range_search_eucledian(vec,g,range,a,b);
    	}
    			
	}

	double HashTable::NN_search_cosine(MyVector* vec,int a,int b)
	{

		int index = vec->compute_cosine_index(v);

    	if(index!=-1)
    	{
    		return Table[index]->search_cosine(vec,a,b);
    	}
    			
	}

	list<MyVector*> HashTable::range_search_cosine(MyVector* vec,double range,int a,int b)
	{

		int index = vec->compute_cosine_index(v);

    	if(index!=-1)
    	{
    		return Table[index]->range_search_cosine(vec,range,a,b);
    	}

    }


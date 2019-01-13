#include "HyperCube.h"

    void Cube::insert_eucledian(MyVector* vec) {

    	int* g = vec->compute_eucledian_g(t,v);

    	int* indx = vec->compute_cube_f(g);

    	int index = convert_bits_to_int(indx);

    	if(index!=-1)
    	{
    		Table[index]->insert(vec,indx);
    	}
	}
    

	void Cube::insert_cosine(MyVector* vec){

		int index = vec->compute_cosine_index(v);

    	if(index!=-1)
    	{
    		Table[index]->insert(vec,NULL);
    	}

	}


	double Cube::NN_search_cosine(MyVector* vec){

        int *ham_dist = new int[TableSize];

		double dist = 0.0;
		double min_dist = 2.0;

		int neighbors_examined = 0;

		int probes_examined = 0;

        if(probes != 0) M = -1;

        int index = vec->compute_cosine_index(v);

        for(int i=0; i<TableSize; i++)
            ham_dist[i] = hammingDistance(index,i);

		while((neighbors_examined < M) || (probes_examined < probes))
		{

    		if(index!=-1)
    		{
    			dist = Table[index]->search_cosine(vec,neighbors_examined,M);
    			if(dist < min_dist)
    				min_dist = dist;
    		}

    		probes_examined++;

    		ham_dist = next_probe(index,TableSize,ham_dist);
            
		}

        delete []ham_dist;

		return min_dist;

	}

	double Cube::range_search_cosine(MyVector* vec,double range, ofstream& outfile){

		int *ham_dist = new int[TableSize];

        int index = vec->compute_cosine_index(v);

		int neighbors_examined = 0;

		int probes_examined = 0;

        if(probes != 0) M = -1;

        for(int i=0; i<TableSize; i++)
            ham_dist[i] = hammingDistance(index,i);


		while((neighbors_examined < M) || (probes_examined < probes))
		{			

    	if(index!=-1)
    	{
    		Table[index]->range_search_cosine(vec,range,outfile,neighbors_examined,M);
    	}

    	probes_examined++;

    	ham_dist = next_probe(index,TableSize,ham_dist);

        }

        delete []ham_dist;
    			
	}


	double Cube::NN_search_eucledian(MyVector* vec){

        int *ham_dist = new int[TableSize];

		double dist = 0.0;
		double min_dist = 100000;

		int neighbors_examined = 0;

		int probes_examined = 0;

    	int *g = vec->compute_eucledian_g(t,v);

    	int* indx = vec->compute_cube_f(g);

    	int index = convert_bits_to_int(indx);

        for(int i=0; i<TableSize; i++)
            ham_dist[i] = hammingDistance(index,i);

        if(probes != 0) M = -1;

		while((neighbors_examined < M) || (probes_examined < probes) )
		{

    		if(index!=-1)
    		{
    			dist = Table[index]->search_eucledian(vec,indx,neighbors_examined,M);
    			if(min_dist > dist)
    				min_dist = dist;
    		}

    		probes_examined++;
    		ham_dist = next_probe(index,TableSize,ham_dist);

    		indx = new int[K];
    		int_to_bin_digit(index,K,indx);

		}

        delete []ham_dist;

		return min_dist;
	}

	void Cube::range_search_eucledian(MyVector* vec, double range, ofstream& outfile){

    	int *ham_dist = new int[TableSize];

        int* g = vec->compute_eucledian_g(t,v);

    	int neighbors_examined = 0;

    	int probes_examined = 0;

    	int* indx = vec->compute_cube_f(g);

    	int index = convert_bits_to_int(indx);

        if(probes != 0) M = -1;

        for(int i=0; i<TableSize; i++)
            ham_dist[i] = hammingDistance(index,i);

    	while((neighbors_examined < M) || (probes_examined < probes) )
    	{
    	   if(index!=-1)
    	   {
    		  Table[index]->range_search_eucledian(vec,indx,range,outfile,neighbors_examined,M);
    	   }

    	   probes_examined++;
    	   ham_dist = next_probe(index,TableSize,ham_dist);

    	   indx = new int[K];
    	   int_to_bin_digit(index,K,indx);

    	}

        delete []ham_dist;
	}
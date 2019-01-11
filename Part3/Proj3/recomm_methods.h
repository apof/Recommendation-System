#ifndef RECOMM_METHODS
#define RECOMM_METHODS

#include "../../Part1/Proj1_updated/Vector.h"
#include "../../Part1/Proj1_updated/HashTable.h"
#include "../../Part2/Proj2_updated/cluster.h"


class help_node{

public:

	long double *val;
	long double dist;
	int* flag;
	int id;
	long double mean_val;

	help_node(MyVector* v,long double d){

		val = new long double[DATA_VECTOR_SIZE];
		flag = new int[DATA_VECTOR_SIZE];

		for(int i=0; i<DATA_VECTOR_SIZE; i++)
		{
			val[i] = v->doubleVecArray[i];
			flag[i] = v->flag[i];
		}

		dist = d;
		id = v->int_id;
		mean_val = v->mean_value;

	}

	//~help_node() { delete []val; delete []flag; }

};

struct help_node_Comparator
{
	bool operator ()(const help_node & node1, const help_node & node2)
	{
		if(node1.dist == node2.dist)
			return node1.dist < node2.dist;
		return node1.dist < node2.dist;
 
	}
};

class result_node{

public:

	int pos;
	long double prediction;
	int flag;

	result_node(int p,long double pred,int f){

		pos = p;
		prediction = pred;
		flag = f;
	}

};

struct result_node_Comparator
{
	bool operator ()(const result_node & node1, const result_node & node2)
	{
		if(node1.prediction == node2.prediction)
			return node1.prediction < node2.prediction;
		return node1.prediction < node2.prediction;
 
	}
};

list<MyVector*> find_single_users(list<MyVector*>);
list<help_node> select_top_neighbors(MyVector*,list<MyVector*>,char* metric);
list<result_node> make_prediction(list<help_node> nodes, help_node prediction_node);
void recommendation_based_on_lsh(MyVector**,int,char*,string* coins,string);


#endif



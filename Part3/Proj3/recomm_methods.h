#ifndef RECOMM_METHODS
#define RECOMM_METHODS

#include "../../Part1/Proj1_updated/Vector.h"
#include "../../Part1/Proj1_updated/HashTable.h"
#include "../../Part2/Proj2_updated/cluster.h"

#define PN 20
#define USERS_TO_RECOMM 4

class help_node{

public:

	long double *val;
	long double *old_val;
	long double dist;
	int* flag;
	int id;
	long double mean_val;

	help_node(MyVector* v,long double d){

		val = new long double[DATA_VECTOR_SIZE];
		flag = new int[DATA_VECTOR_SIZE];
		old_val = new long double [DATA_VECTOR_SIZE];

		for(int i=0; i<DATA_VECTOR_SIZE; i++)
		{
			val[i] = v->doubleVecArray[i];
			flag[i] = v->flag[i];
			old_val[i] = v->doubleVecArray[i];
		}

		dist = d;
		id = v->int_id;
		mean_val = v->mean_value;

	}

	void set_unknown(){

		int known_number = 0;
		for(int i=0; i<DATA_VECTOR_SIZE; i++)
		{
			if(flag[i]==1) known_number++;
		}
		if(known_number==1) return;

		int number_to_hide = known_number/2;

		int index = 1;

		for(int i=0; i<DATA_VECTOR_SIZE; i++)
		{
			if(flag[i]==1){

				flag[i] = 2;
				val[i] = mean_val;
				index++;
			}

			if(index==number_to_hide+1)
				break;
		}
	}

	void print_help_node(){

		
		cout<<id<<" "<<dist<<" "<<mean_val<<endl;

		for(int i=0; i<DATA_VECTOR_SIZE; i++)
		{
			cout<<val[i]<<" ";
		}
		cout<<endl;
		for(int i=0; i<DATA_VECTOR_SIZE; i++)
		{
			cout<<flag[i]<<" ";

		}
		cout<<endl;
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
	long double old_val;

	result_node(int p,long double pred,int f,long double ov){

		pos = p;
		prediction = pred;
		flag = f;
		old_val = ov;
	}

};

struct result_node_Comparator
{
	bool operator ()(const result_node & node1, const result_node & node2)
	{
		if(node1.prediction == node2.prediction)
			return node1.prediction > node2.prediction;
		return node1.prediction > node2.prediction;
 
	}
};

list<MyVector*> find_single_users(list<MyVector*>);
list<help_node> select_top_neighbors(MyVector*,list<MyVector*>,char* metric);
list<result_node> make_prediction(list<help_node> nodes, help_node prediction_node);
void recommendation_based_on_lsh(MyVector**,int,char*,string* coins,string);
void recommendation_based_on_clustering(MyVector**,int,char*,string* coins,string,int,int,int,int);
void validation_on_lsh(MyVector**,int,char*);


#endif



#ifndef CLUSTER_INCLUDED
#define CLUSTER_INCLUDED

#include "../../Part1/Proj1_updated/Vector.h"
#include "../../Part1/Proj1_updated/HashTable.h"
#include "../../Part1/Proj1_updated/HyperCube.h"

class cluster_node{

public:

	MyVector* Vector;

	cluster_node(MyVector* v) {
		Vector = v; 
	}

	~cluster_node(){
		Vector = NULL;
	}

};


class Cluster{

private:

	list<cluster_node> cluster_list;
	MyVector* center;
	char* metric;

public:

	Cluster(char* m){

		center = NULL;
		metric = m;
	}

	/*~Cluster(){

		if (center!=NULL)
			delete center;
			center = NULL;
	}*/


	void add_to_cluster(MyVector* vec){

		cluster_node node(vec);
		cluster_list.push_back(node);
	}

	void cluster_print(){

		for (auto n : cluster_list)
        n.Vector->PrintVector();
	}

	void set_center(MyVector* c){
		center = c;
	 }

	MyVector* get_center(){
	 	return center;
	 }

	int get_size(){

		return cluster_list.size();
	}

	void clc(){

		cluster_list.clear();
	}

	list<cluster_node> get_list(){

		return cluster_list;
	}



	//update methods
	long double Kmeans_update(int);
	long double Pam_Improved_like_Lloyd_update();
	long double Pam_using_distances(unordered_map<string, long double>);

};

class Cluster_Table{

private:

	int cluster_number;
	Cluster** clusters;
	char* metric;
	int iteration;
	unordered_map<string, long double> dist_map;
	int initialization;
	int assignement;
	int update;
	int l;
	int complete;



public:

	Cluster_Table(int num,char* m,int in,int ass,int up,int ll,int comp){

		cluster_number = num;
		clusters = new Cluster*[cluster_number];
		for(int i=0; i<cluster_number; i++)
			clusters[i] = new Cluster(m);

		metric = m;
		iteration = 0;

		initialization = in;
		assignement = ass;
		update = up;

		l = ll;

		complete = comp;

	}

	~Cluster_Table(){

		for(int i=0; i<cluster_number; i++)
			delete clusters[i];
		delete []clusters;
	}

	Cluster** get_clusters(){

		return clusters;
	}

	void print_centers(){

		for(int i=0; i<cluster_number; i++){

			clusters[i]->get_center()->PrintVector();
		}
	}

	void get_size(){

		for (int i = 0; i < cluster_number; i++)
		{
			cout<<"Cluster "<<i<<": "<<clusters[i]->get_size()<<endl;;
		}
	}

	void clear_lists(){

		for (int i = 0; i < cluster_number; i++)
		{
			clusters[i]->clc();
		}
	}

	//initialization methods
	void random_initialization(MyVector**);
	void kmeans_plus_plus_initialization(MyVector**);

	//assignement methods
	void Lloyds_assignment(MyVector**);
	void Range_Search_LSH_assignment(MyVector**,LSH_Tables*);
	void Range_Search_Cube_assignment(MyVector**,Cube*);


	//update methods
	long double Kmeans_update(int iteration){

		long double total = 0.0;

		for(int i=0; i<cluster_number; i++){

			long double t = clusters[i]->Kmeans_update(iteration);
			//cout<<t<<endl;
			total += t;
		}

		return total;
	}

	long double Pam_Improved_like_Lloyd_update(){

		long double total = 0.0;

		for(int i=0; i<cluster_number; i++){

			total += clusters[i]->Pam_Improved_like_Lloyd_update();
		}

		return total;
	}

	long double Pam_using_distances(){

		long double total = 0.0;

		for(int i=0; i<cluster_number; i++){

			total += clusters[i]->Pam_using_distances(dist_map);
		}

		return total;
	}

	void clustering(MyVector** vec_pointers){

	cout<<"okk"<<endl;
	
	if(initialization==1)
		random_initialization(vec_pointers);
	else
		kmeans_plus_plus_initialization(vec_pointers);

	long double flag = 100;
	int iter = 0;

	LSH_Tables* Tables;
	Cube* C;

	if(assignement == 2)
	{
		Tables = new LSH_Tables(l);
		for(int i =0; i<DATA_NUMBER; i++)
		{
			if(strcmp(metric,"eucledian")==0)
    			Tables->insert_eucledian(vec_pointers[i]);
    		else if (strcmp(metric,"cosine")==0)
    			Tables->insert_cosine(vec_pointers[i]);
		}
	}
	else if(assignement == 3)
	{
		C = new Cube(10,100,K);
		for(int i =0; i<DATA_NUMBER; i++)
		{
			if(strcmp(metric,"eucledian")==0)
    			C->insert_eucledian(vec_pointers[i]);
    		else if (strcmp(metric,"cosine")==0)
    			C->insert_cosine(vec_pointers[i]);
		}
	}


	
	while(iter!=MAX_ITERS && flag > 0.0)
	{

		if(assignement==1)
			Lloyds_assignment(vec_pointers);
		else if(assignement==2)
			Range_Search_LSH_assignment(vec_pointers,Tables);
		else if(assignement==3)
			Range_Search_Cube_assignment(vec_pointers,C);

		if(update==1)
			flag = Kmeans_update(iteration);
		else if(update==2)
			//flag = Pam_Improved_like_Lloyd_update();
			flag = Pam_using_distances();

		cout<<iter<<" "<<flag<<endl;

		iter++;
	}

	if(assignement == 2)
		delete Tables;
	if(assignement == 3)
		delete C;

	}


void compute_silhouette(ofstream&,long double);
  
};

#endif
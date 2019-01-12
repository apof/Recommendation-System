#include "recomm_methods.h"

list<MyVector*> find_single_users(list<MyVector*> l){

	// exclude duplicate users from range search list
	// a user may appear two or more times in the list since it has been found in more than one lsh buckets

	unordered_map<int,MyVector*> map;

	for(auto n : l)
	{
		unordered_map<int,MyVector*>::iterator it;

		it = map.find(n->int_id);

		if (it == map.end())
		{
			map[n->int_id] = n;
		}
	}

	list<MyVector*> ret_list;

	unordered_map<int,MyVector*>::iterator pos;
    for (pos = map.begin(); pos != map.end(); ++pos) {
        ret_list.push_back(pos->second);
    }

	return ret_list;
}

list<help_node> select_top_neighbors(MyVector* v,list<MyVector*> l,char* metric){

	list<help_node> nodes;

	// create a copy list of neighbors in order not to overwrite existing dataset
	// and compute their distance from the user we want to predict
	for(auto elem : l)
	{
		nodes.push_back(help_node(elem,v->compute_distance(elem,metric)));
	}

	// sort the list according to the distance (ascending order)
	nodes.sort(help_node_Comparator());

	// take >= to_20 users
	list<help_node> result_nodes;
	int index = 1;
	for(auto e : nodes)
	{
		if(e.dist!=0)
		{
			result_nodes.push_back(e);
			index++;

		}

		if(index==PN+1) break;

	}

	return result_nodes;

}


list<result_node> make_prediction(list<help_node> nodes, help_node prediction_node){

	list<result_node> result_list;

	// compute total abs similarity and normalizing factor z
	long double total_abs_similarity = 0.0;

	for(auto elem : nodes)
	{
		total_abs_similarity += (1-elem.dist);
	}

	long double z = 1/abs(total_abs_similarity);


	// make prediction for every coin of the user
	// at the same time we normalize the values (two different normalizations for known and unknown coins)
	for(int i=0; i<DATA_VECTOR_SIZE; i++)
	{
		if(prediction_node.flag[i]==1)
			prediction_node.val[i] -= prediction_node.mean_val;
		else
		{
			long double plus_value = 0.0;

			for(auto neighbor : nodes)
			{
				plus_value += (1-neighbor.dist)*(neighbor.val[i]-neighbor.mean_val);
			}

			prediction_node.val[i] += z*plus_value;
		}
	}


	// create a result list
	for(int i=0; i<DATA_VECTOR_SIZE; i++)
	{
		result_node n(i,prediction_node.val[i],prediction_node.flag[i]);

		result_list.push_back(n);
	}

	// sort the list (descending order)
	result_list.sort(result_node_Comparator());


	// return top 5 unknown coins
	list<result_node> final_res;
	int index = 1;
	for(auto e : result_list)
	{
		if(e.flag==0)
		{
			final_res.push_back(e);
			index++;

		}

		if(index==6) break;

	}

	return final_res;

	

}

void recommendation_based_on_lsh(MyVector** vec_pointers,int l,char* metric,string* coin_array,string output_file)
{

	cout<<"Recommendation based on lsh.."<<endl;
	
	ofstream outfile(output_file);

	outfile<<"Recommendation based on lsh..."<<endl;

	// insert data to lsh tables
	LSH_Tables* Tables = new LSH_Tables(l);
	for(int i=0; i<DATA_NUMBER; i++)
	{
		Tables->insert_cosine(vec_pointers[i]);
  	}

  	// for every user we want to predict its unknown coins
  	for(int i=0; i<USERS_TO_RECOMM; i++)
	{
		//find his top neighbors
		list<MyVector*> range_users = find_single_users(Tables->range_search_cosine(vec_pointers[i],0.05));
  		list<help_node> nodes =  select_top_neighbors(vec_pointers[i],range_users,metric);

  		list<result_node> result_list;
  		if(nodes.size()!=0)
  		{
  			result_list = make_prediction(nodes,help_node(vec_pointers[i],0));

  			outfile<<"User "<<vec_pointers[i]->int_id<<" predictions: "<<" ";

  			for(auto elem : result_list){

  				outfile<<coin_array[elem.pos]<<"-"<<elem.prediction<<" ";
  			}

  			outfile<<endl;

  		}
  		else
  		{
  			outfile<<"No neighbors found for user: "<<vec_pointers[i]->int_id<<endl;
  		}

  		
  	}
  
  	delete Tables;

  	outfile.close();

}

void recommendation_based_on_clustering(MyVector** vec_pointers,int l,char* metric,string* coin_array,string output_file,int cluster_num,int initialization,int assignment,int update)
{
	cout<<"Clustering.."<<endl;

	Cluster_Table* Ctable = new Cluster_Table(cluster_num,metric,initialization,assignment,update,l,0);
  	Ctable->clustering(vec_pointers);

  	Cluster** clusters = Ctable->get_clusters();

  	cout<<"Recommendation based on clustering..."<<endl;

  	ofstream outfile(output_file);

  	outfile<<"Recommendation based on clustering..."<<endl;

  	list<cluster_node> n;

  	//make prediction for the users of one of the clusters

  	for(int i=0; i<cluster_num; i++)
  	{
  		if((clusters[i]->get_list()).size() > 20)
  			n = clusters[i]->get_list();
  	}

  	list<MyVector*> neighbors;

  	for(auto elem : n){

  		neighbors.push_back(elem.Vector);
  	}

  	for(auto elem : neighbors)
  	{
  		for(int j=0; j<DATA_NUMBER; j++)
  		{
  			if(elem->int_id == vec_pointers[j]->int_id)
  			{

  				list<help_node> nodes =  select_top_neighbors(vec_pointers[j],neighbors,metric);
  				list<result_node> result_list;

  				if(nodes.size()!=0)
  				{
  					result_list = make_prediction(nodes,help_node(vec_pointers[j],0));

  					outfile<<"User "<<vec_pointers[j]->int_id<<" predictions: "<<" ";

  					for(auto elem : result_list){
  						outfile<<coin_array[elem.pos]<<"-"<<elem.prediction<<" ";
  					}
  					outfile<<endl;

  				}
  				else
  				{
  					outfile<<"No neighbors found for user: "<<vec_pointers[j]->int_id<<endl;
  				}

  			}
  		}
  	}

  	delete Ctable;

  	outfile.close();
}
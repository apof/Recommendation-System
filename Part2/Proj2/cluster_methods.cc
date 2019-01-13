#include "cluster.h"

void Cluster_Table::random_initialization(MyVector** v){

		int flag;
		flag = 1;

		int* c = new int[cluster_number];

		//while loop in order to choose distinct centroids
		while(flag==1)
		{
			// choose random elements as centroids
			for (int t=0;t<cluster_number;t++)
			{
    			c[t] = rand() % DATA_NUMBER;
    		}

    		int origVal = 0, newVal = 0;


    		// check them to be distinct
    		for (int i = 0; i < cluster_number; i++)
    		{
        	origVal = c[i];

        		for (int k = i+1; k < cluster_number; k++)
        		{

            	if (origVal == c[k])
            	{
                	newVal = 1;
                	break;
            	}

        		}

        	if (newVal ){break;}

    		}

    	if (newVal == 1) flag = 1;
    	else flag = 0;

    	}

    	// cluster center initialization
    	for(int i=0; i<cluster_number; i++)
    	{
    		clusters[i]->set_center(v[c[i]]);
    	}

    	delete[] c;
    		

	}

void Cluster_Table::kmeans_plus_plus_initialization(MyVector** v){

	int *centroid_indexes = new int[cluster_number];

	//choose the first centroid randomly from the points
	int r = rand() % DATA_NUMBER;
	centroid_indexes[0] = r;
	clusters[0]->set_center(v[r]);

	double *D =  new double[DATA_NUMBER-1];
	double *P =  new double[(DATA_NUMBER - 1) + 1];


	for(int t=1; t<cluster_number; t++)
	{

	//cout<<t<<endl;

	// compute all D: distance of every non centroid point from the closest centroid 
	int index = 0;
	for(int i=0; i<DATA_NUMBER; i++)
	{
		//check point to be non centroid
		int flag = 1;
		for(int k=0; k<t; k++)
		{
			if(i==centroid_indexes[k])
			{
				flag = 0;
				break;
			}
		}

		//compute min distance
		if(flag == 1)
		{

			double min = numeric_limits<double>::max();
			double dist;

			for(int l=0; l<t; l++)
			{
				dist = clusters[l]->get_center()->compute_distance(v[i],metric);
				if(dist < min)
					min = dist;
			}

			D[index] = min;
			index++;
		}
	}

	double max = 0.0;
	// find max Di
	for(int i =0; i<DATA_NUMBER-t; i++)
		if(D[i] > max)
		{
			max = D[i];
		}

	//Di normalization
	for(int i =0; i<DATA_NUMBER-t; i++)
		D[i] = D[i]/max;
	
	P[0] = 0.0;
	for (int i = 1; i < DATA_NUMBER-t; i++)
	{
		P[i] = P[i-1] + (D[i-1]*D[i-1]);
	}

	// create a random number x in [0,P(n-t)]
	float x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(P[(DATA_NUMBER-t-1)])));

	//choose r as P(r-1) < x =< P(r), and select this r as next centroid
	int r;
	for(int i = 1; i < DATA_NUMBER-t; i++)
	{
		if(P[i-1] < x && x <= P[i])
		{
			r = i;
			break;
		}

	}

	//find the real centroid index in data table in order to take the wright point
	for(int i=0; i<t; i++)
	{
		if(centroid_indexes[i] <= r)
			r++;
	}

	clusters[t]->set_center(v[r]);
	centroid_indexes[t]  = r;

	}

	delete []centroid_indexes;
	centroid_indexes = NULL;
	delete []P;
	P = NULL;
	delete []D;
	D = NULL;

	}


void Cluster_Table::Lloyds_assignment(MyVector** v){

	clear_lists();

	for(int i=0; i<DATA_NUMBER; i++){

		int index;
		double min = numeric_limits<double>::max();;
		double dist; 
		for(int j=0; j<cluster_number; j++){

			dist = (clusters[j]->get_center())->compute_distance(v[i],metric);
			if(dist<min){
				min = dist;
				index = j;
			}
		}

		clusters[index]->add_to_cluster(v[i]);
	}

	iteration++;
}

void Cluster_Table::Range_Search_LSH_assignment(MyVector** v,LSH_Tables* Tables){

	clear_lists();

	unordered_map<string,int> assign_map;

	long double min_centers_dist = numeric_limits<double>::max();

	long double dist;
	for(int i=0; i<cluster_number; i++)
	{
		for(int j=0; j<cluster_number; j++)
		{
			dist = (clusters[j]->get_center())->compute_distance((clusters[i]->get_center()),metric);
			if(dist!=0 && dist < min_centers_dist) min_centers_dist = dist;
		}
	}

	double range = min_centers_dist/2;

	int points_assigned = 0;

	int minimum = 0.7*DATA_NUMBER;

	while(points_assigned<minimum)
	{

		for(int j=0; j<cluster_number; j++)
		{
			list<MyVector*> l;
			if(strcmp(metric,"cosine")==0)
				l = Tables->range_search_cosine(clusters[j]->get_center(),range);
			else
			{
				l = Tables->range_search_eucledian(clusters[j]->get_center(),range);
			}

			for (auto n : l)
        	{

        		unordered_map<string,int>::iterator it;
				it = assign_map.find(n->id);

				if (it != assign_map.end())
				{
					if(it->second!=j)
					{
						long double d1 = (clusters[it->second]->get_center())->compute_distance(n,metric);
						long double d2 = (clusters[j]->get_center())->compute_distance(n,metric);
						if(d2<d1) assign_map[n->id] = j;

					}
				}
				else { points_assigned++; assign_map[n->id] = j; }

        	}

		}

		range = range*2;

	}

	for(int i=0; i<DATA_NUMBER; i++)
	{

		unordered_map<string,int>::iterator it;
		it = assign_map.find(v[i]->id);

		if (it != assign_map.end())
			clusters[it->second]->add_to_cluster(v[i]);		
		else {

			long double m_dist = numeric_limits<double>::max();
			int index;

			for(int k=0; k<cluster_number; k++)
			{
				long double d = v[i]->compute_distance(clusters[k]->get_center(),metric);
				if(d<m_dist)
				{
					m_dist = d;
					index = k;
				}
			}

			clusters[index]->add_to_cluster(v[i]);

		}

	}

	iteration++;
}


void Cluster_Table::Range_Search_Cube_assignment(MyVector** v,Cube* C){

	clear_lists();

	unordered_map<string,int> assign_map;

	long double min_centers_dist = numeric_limits<double>::max();

	long double dist;
	for(int i=0; i<cluster_number; i++)
	{
		for(int j=0; j<cluster_number; j++)
		{
			dist = (clusters[j]->get_center())->compute_distance((clusters[i]->get_center()),metric);
			if(dist!=0 && dist < min_centers_dist) min_centers_dist = dist;
		}
	}

	double range = min_centers_dist/2;

	int points_assigned = 0;

	int minimum = 0.7*DATA_NUMBER;

	while(points_assigned<minimum)
	{

		for(int j=0; j<cluster_number; j++)
		{
			list<MyVector*> l;
			if(strcmp(metric,"cosine")==0)
				l = C->range_search_cosine(clusters[j]->get_center(),range);
			else
			{
				l = C->range_search_eucledian(clusters[j]->get_center(),range);
			}

			for (auto n : l)
        	{

        		unordered_map<string,int>::iterator it;
				it = assign_map.find(n->id);

				if (it != assign_map.end())
				{
					if(it->second!=j)
					{
						long double d1 = (clusters[it->second]->get_center())->compute_distance(n,metric);
						long double d2 = (clusters[j]->get_center())->compute_distance(n,metric);
						if(d2<d1) assign_map[n->id] = j;

					}
				}
				else { points_assigned++; assign_map[n->id] = j; }

        	}

		}

		range = range*2;

	}

	for(int i=0; i<DATA_NUMBER; i++)
	{

		unordered_map<string,int>::iterator it;
		it = assign_map.find(v[i]->id);

		if (it != assign_map.end())
			clusters[it->second]->add_to_cluster(v[i]);		
		else {

			long double m_dist = numeric_limits<double>::max();
			int index;

			for(int k=0; k<cluster_number; k++)
			{
				long double d = v[i]->compute_distance(clusters[k]->get_center(),metric);
				if(d<m_dist)
				{
					m_dist = d;
					index = k;
				}
			}

			clusters[index]->add_to_cluster(v[i]);

		}

	}

	iteration++;
}


long double Cluster::Kmeans_update(int iteration){


	int cluster_size = get_size();

	if(cluster_size!=0)
	{
	long double* med = new long double[DATA_VECTOR_SIZE];
	for(int k=0; k<DATA_VECTOR_SIZE; k++){
		med[k] = 0.0;
	}

	for (auto n : cluster_list)
	{
        long double* vec = n.Vector->doubleVecArray;
        for(int j=0; j<DATA_VECTOR_SIZE; j++)
        {
        	med[j] += vec[j];
        }
	}

	for(int i=0; i<DATA_VECTOR_SIZE; i++)
	{
		med[i] = med[i]/cluster_size;
	}

	string id = "medID";
    string type("double");

    MyVector* median = new MyVector(type,id);

    median->set_vector(med);

    long double d = median->compute_distance(center,metric);

    //if (iteration!=1){
    //	delete center;
    //	center = NULL;
    //}

    //if(isnan(d))
    //{
    //	center->PrintVector();
    //	median->PrintVector();
    //}

    center = median;

    return d;

	}

	return 0.0;

}

long double Cluster::Pam_Improved_like_Lloyd_update(){

	long double objective_func;
	double min_dist = numeric_limits<double>::max();
	MyVector* c;

	if(get_size()!=0)
	{

	for (auto n : cluster_list)
	{
      

	objective_func = 0.0;
	for (auto nn : cluster_list)
	{
		objective_func += nn.Vector->compute_distance(n.Vector,metric);
	}  

	if(objective_func<min_dist)
	{
		min_dist = objective_func;
		c = n.Vector;

	}

	}

	long double d = c->compute_distance(center,metric);

	center = c;

	return d;

	}

	return 0.0;
}

long double Cluster::Pam_using_distances(unordered_map<string, long double> dist_map){

	long double objective_func;
	double min_dist = numeric_limits<double>::max();
	MyVector* c;

	if(get_size()!=0)
	{

	for (auto n : cluster_list)
	{
      
	objective_func = 0.0;

	for (auto nn : cluster_list)
	{
		unordered_map<string,long double>::iterator it;
		it = dist_map.find(n.Vector->id+"-"+nn.Vector->id);
		if (it != dist_map.end())
			objective_func += it->second;
		else
		{
			long double dist = nn.Vector->compute_distance(n.Vector,metric);
			objective_func += dist;
			dist_map[n.Vector->id+"-"+nn.Vector->id] = dist;

		}
	}  

	if(objective_func<min_dist)
	{
		min_dist = objective_func;
		c = n.Vector;

	}

	}

	long double d = c->compute_distance(center,metric);

	center = c;

	return d;

	}

	return 0.0;

}

void Cluster_Table::compute_silhouette(ofstream& outfile,long double time){

	cout<<"Computing Silhouette..."<<endl;

	outfile<<"Algorithm: "<<"I"<<initialization<<"A"<<assignement<<"U"<<update<<endl;
	outfile<<"Metric: "<<metric<<endl;
	for(int i=0; i<cluster_number; i++)
	{
		outfile<<"Cluster "<<i<<": { size: "<<clusters[i]->get_size()<<endl;
		if(update==2)
			outfile<<"centroid: "<<(clusters[i]->get_center())->id<<" }"<<endl;
		else if (update==1)
		{
			outfile<<"centroid: ";
			for(int j=0; j<DATA_VECTOR_SIZE; j++)
			{
				outfile<<(clusters[i]->get_center())->doubleVecArray[j]<<" ";
			}
			outfile<<" }"<<endl;
		}
	}

	long double** sils;
	sils = new long double*[cluster_number];
	for(int i=0; i<cluster_number; i++)
	sils[i] = new long double[clusters[i]->get_size()];


	for(int i=0; i<cluster_number; i++)
	{
		list<cluster_node> cluster_list = clusters[i]->get_list();
		int index = 0;
		for (auto n : cluster_list)
		{
			long double average_dist = 0.0;

			for (auto nn : cluster_list)
			{
			average_dist += n.Vector->compute_distance(nn.Vector,metric);
			}

			average_dist = average_dist/clusters[i]->get_size();

			int next_index;
			long double min_dist = numeric_limits<double>::max();
			for(int k=0; k<cluster_number; k++)
			{
				if(k!=i)
				{
					long double dist = n.Vector->compute_distance(clusters[k]->get_center(),metric);
					if (dist<min_dist)
					{
						min_dist = dist;
						next_index = k;
					}
				}

			}

			long double next_average_dist = 0.0;

			list<cluster_node> cluster_list2 = clusters[next_index]->get_list();

			for (auto nn : cluster_list2)
			{
			next_average_dist += n.Vector->compute_distance(nn.Vector,metric);
			}

			next_average_dist = next_average_dist/clusters[next_index]->get_size();

			long double max;

			if(next_average_dist > average_dist)
				max = next_average_dist;
			else
				max = average_dist;

			sils[i][index] = (next_average_dist - average_dist)/max;

			index++;


		}

		
	}

	outfile<<"Clustering time: "<<time<<endl;

	long double total_sil = 0.0;

	for(int i=0; i<cluster_number; i++)
	{
		long double avg_sil = 0.0;
		int clust_num = clusters[i]->get_size();

		for(int j=0; j<clust_num; j++)
		{
			avg_sil += sils[i][j];
		}

		avg_sil = avg_sil/clust_num;

		outfile<<"Silhouette of cluster "<<i<<" "<<avg_sil<<endl;

		total_sil += avg_sil;

	}

	total_sil = total_sil/cluster_number;

	outfile<<"Total Silhouette: "<<total_sil<<endl;

	if(complete==1)
	{
		for(int i=0; i<cluster_number; i++)
		{
			outfile<<"Cluster: "<<i<<" ";
			list<cluster_node> l = clusters[i]->get_list();
			for (auto n : l)
        		outfile<<n.Vector->id<<" ";
        	outfile<<endl;

		}
	}

}

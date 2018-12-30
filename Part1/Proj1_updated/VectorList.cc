#include "VectorList.h"

	double VectorList::search_eucledian(MyVector* vec,int* gg, int &neighbors_examined, int M){

		double dist = 0.0;
		double min_dist = 100000;

		ListNode* tmp = root;

		while(tmp!=NULL){


			int flag = ckeck_same_g(gg,tmp->g);
			//int flag = 1;
			
			if(flag==1)
			{

				neighbors_examined++;

				dist = vec->eucledian_distance(tmp->vector);
				if(min_dist>dist)
					min_dist = dist;
			}

			if(neighbors_examined == M) { return min_dist; }

			tmp = tmp->next;
		}

		delete []gg;

		return min_dist;

	}


	double VectorList::search_cosine(MyVector* vec,int &neighbors_examined, int M){

		double dist = 0.0;
		double min_dist = 2.0;

		ListNode* tmp = root;

		while(tmp!=NULL){

				neighbors_examined++;

				dist = cosine_similarity(tmp->vector->doubleVecArray,vec->doubleVecArray);
				if(dist < min_dist)
					min_dist = dist;

			if(neighbors_examined == M) { return min_dist; }

			tmp = tmp->next;

		}

		return min_dist;

}



	list<MyVector*>  VectorList::range_search_eucledian(MyVector* vec,int* gg,double range,int &neighbors_examined, int M)
	{

		double dist = 0.0;

		ListNode* tmp = root;

		list<MyVector*> range_list;

		while(tmp!=NULL){

				neighbors_examined++;

				dist = vec->eucledian_distance(tmp->vector);
				if(dist <= range)
					range_list.push_back(tmp->vector);

			if(neighbors_examined == M) { return range_list; }

			tmp = tmp->next;
		}

		delete []gg;

		return range_list;

	}

	  list<MyVector*> VectorList::range_search_cosine(MyVector* vec,double range,int &neighbors_examined, int M){


		double dist = 0.0;

		ListNode* tmp = root;

		list<MyVector*> range_list;

		while(tmp!=NULL){

				neighbors_examined++;

				dist = cosine_similarity(tmp->vector->doubleVecArray,vec->doubleVecArray);
				if(dist <= range)
					range_list.push_back(tmp->vector);

				if(neighbors_examined == M) { return range_list; }

			tmp = tmp->next;
		}

		return range_list;

	}
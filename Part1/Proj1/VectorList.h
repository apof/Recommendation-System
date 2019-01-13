#ifndef VECTORLIST_INCLUDED
#define VECTORLIST_INCLUDED

#include "Vector.h"

class ListNode {

public:

	ListNode* next;
	MyVector* vector;
	int* g;

	ListNode(MyVector* vec,int* gg){

		vector = vec;
		g = gg;
		next = NULL;

	}

	~ListNode() { 

		if(g!=NULL)
		{ 
			delete []g; 
			g = NULL;
		}

		vector = NULL; 
		next = NULL; 
	}

	void ListNodePrint() { 
		vector->PrintVector();

		if (g!=NULL)
		{
		cout<<"Vector's g: "<<endl; 
		for(int i = 0; i<K; i++)
			cout<<g[i]<<" ";
		}
		cout<<endl; 
	}

};

class VectorList{

private:

	ListNode* root;
	int list_size;

public:

	int get_size() { return list_size; }

	VectorList() {

		root = NULL;
		list_size = 0;
	}

	~VectorList() {

		ListNode* tmp;

		while(root!=NULL)
		{
			tmp = root->next;
			delete root;
			root = NULL;
			root = tmp;
		}

	}

	void insert(MyVector* vec,int* gg){

		list_size++;

		if(root==NULL) root = new ListNode(vec,gg);
		else
		{
			ListNode* temp = root;
			root = new ListNode(vec,gg);
			root->next = temp;
		}
	}

	double search_eucledian(MyVector*,int*,int&, int);

	double search_cosine(MyVector*,int&,int);

	void  range_search_eucledian(MyVector*,int*,double,ofstream&,int&,int);

	void  range_search_cosine(MyVector*,double,ofstream&,int&, int);


	void print_list()
	{

		cout<<"Size of the list:   "<<list_size<<endl;
		ListNode* temp = root;
		while(temp!=NULL)
		{
			temp->ListNodePrint();
			temp = temp->next;
		}

	}

};

#endif
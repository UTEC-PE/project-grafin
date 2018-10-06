#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>

#include "node.h"
#include "edge.h"

using namespace std;

class Traits {
	public:
		typedef int N;
		typedef int E;
};

template <typename Tr>
class Graph {
    public:
        typedef Graph<Tr> self;
        typedef Node<self> node;
        typedef Edge<self> edge;
        typedef vector<node*> NodeSeq;
        typedef list<edge*> EdgeSeq;
        typedef typename Tr::N N;
        typedef typename Tr::E E;
				//Renombrar iteradores
        typedef typename NodeSeq::iterator NodeIte;
        typedef typename EdgeSeq::iterator EdgeIte;

    private:
        NodeSeq nodes; //Vector de punteros de nodos
				//Iteradores
        NodeIte ni=nodes.begin();
        EdgeIte ei;
        int sizeOfGraph[2]= {0,0}; // sizeOfGraph[0]: # de nodes
																	 // sizeOfGraph[1]: # de edges

	public:
	Graph(int size) {
		sizeOfGraph[0] = size;
		node* newnode;
		for (int i=0;i<size;++i){
			newnode=new node(i);
			nodes.push_back(newnode);
		}
	}


	bool add_edge(int Vi, int Vf, int peso, int dir){
		node* initial_node=nodes.at(Vi);
		node* final_node=nodes.at(Vf);

		if(!(initial_node && final_node))
			return false;

		edge* new_edge = new edge(initial_node,nodes[Vf],peso,dir);
		initial_node->add_edge(new_edge);
		++sizeOfGraph[1];
		return true;
	}

	int* size(){ return sizeOfGraph; }

	// Solo para debugging
	void print(){
		for (int i=0; i<nodes.size(); ++i){
			cout<< "\nNodo " << i << ": ";
			for (auto it=nodes[i]->edges.begin(); it!=nodes[i]->edges.end(); it++){
				cout << (*it)->nodes[1]->get_data() << " ";
			}
		}
	}
};

typedef Graph<Traits> graph;

#endif
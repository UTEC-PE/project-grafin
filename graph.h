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
        NodeIte ni;
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

		// comprobar que los vertices existan
		if (!(initial_node && final_node))
			return false;

		edge* new_edge = new edge(initial_node,nodes[Vf],peso,dir);

		if (initial_node->edges.empty()) { initial_node->edges.push_back(new_edge); ++sizeOfGraph[1]; return true; }

		auto edge_in_edges = initial_node->edges.begin();

		// para mantener los edges ordenados en node.edges
		while (edge_in_edges!=initial_node->edges.end() && *new_edge>**edge_in_edges) ++edge_in_edges;

		if (*edge_in_edges==*initial_node->edges.end()) initial_node->edges.push_back(new_edge); // el nuevo edge debe ir al final
		else if (*new_edge==**edge_in_edges) return false; 					// hay otro edge con un mismo inicio y fin
		else initial_node->edges.insert(edge_in_edges, new_edge);

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
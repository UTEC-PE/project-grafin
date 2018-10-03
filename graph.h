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
        int sizeOfGraph;
	public:
	Graph(int size):sizeOfGraph(size){
		node* newnode;
		for (int i=0;i<size;++i){
			newnode=new node(i);
			nodes.push_back(newnode);

		}
	}

};

typedef Graph<Traits> graph;

#endif
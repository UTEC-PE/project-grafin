#ifndef EDGE_H
#define EDGE_H

#include "node.h"

template <typename G>
class Edge {
    public:
        typedef typename G::E E; //saca E de Graph
        typedef typename G::node node;
        //entre que nodos esta
        node* nodes[2];

    private:
        E data; //E es tipo int
        int peso;
        bool dir;

public:
    Edge(node* Vi,node* Vf,int peso, bool dir):peso(peso),dir(dir){
            nodes[0]=Vi;
            nodes[1]=Vf;
    }

    bool operator==(Edge<G> cmp){
      return nodes[0] == cmp.nodes[0] &&
             nodes[1] == cmp.nodes[1];
    }

    bool operator>(Edge<G> cmp){
      return nodes[1]>cmp.nodes[1];
    }

    bool operator<(Edge<G> cmp){
      return nodes[1]<cmp.nodes[1];
    }



};

#endif

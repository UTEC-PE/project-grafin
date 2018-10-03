#ifndef EDGE_H
#define EDGE_H

#include "node.h"

template <typename G>
class Edge {
    public:
        typedef typename G::E E; //saca E de Graph
        typedef typename G::node node;

        node* nodes[2];

    private:
        E data; //E es tipo int
        bool dir;
};

#endif
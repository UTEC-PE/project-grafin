#ifndef NODE_H
#define NODE_H

template <typename G>
class Node {
    public:
        //obtener tipos de Graph
        typedef typename G::N N;
        typedef typename G::E E;
        typedef typename G::edge edge;
        typedef typename G::EdgeSeq EdgeSeq;


    private:
        N data; //N es tipo  entero
        //double x;
        //double y;

    public:
        EdgeSeq edges;

        Node(N nombre):data(nombre){};
        // Métodos de acceso
        N get_data(){ return data; }

};

#endif
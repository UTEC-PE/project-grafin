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
       
    public:
        double x=0;
        double y=0;
        EdgeSeq edges;
        bool thereis= false;

        Node(N nombre):data(nombre){};
        Node(N nombre, double _x, double _y):data(nombre), x(_x), y(_y){};

        int gradoEntrada=0;
        int gradoSalida=0;

        // 0 fuente (entrada)
        // 1 hundido (salida)
        // 2 no tiene tipo
        int get_tipo(){
            return (!gradoEntrada == !gradoSalida) ? 2 : !gradoEntrada;
        }

        // Métodos de acceso
        N get_data(){ return data; }

        ~Node(){
            while(!edges.empty()) {
                delete edges.back();
                edges.pop_back();
            }
        }

};

#endif
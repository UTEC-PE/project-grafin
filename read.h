#ifndef READ_H
#define READ_H

#include "graph.h"

/**
 * Clase de ayuda para leer el grafo desde un archivo,
 * no es necesario que la utilicen, podrían implementar su lector
 * desde su grafo o algún otro lado
 **/
template <typename G>
class Read {
	typedef typename G::N N;
	typedef typename G::E E;
		
	public:
		Read(char* file) {
            // TODO
        }
		
		graph& getGraph() {
            // TODO
        }
};

#endif
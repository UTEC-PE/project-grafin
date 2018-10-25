#ifndef READ_H
#define READ_H

#include<fstream>
#include<string>

#include "graph.h"


/**
 * Clase de ayuda para leer el grafo desde un archivo,
 * no es necesario que la utilicen, podrían implementar su lector
 * desde su grafo o algún otro lado
 **/

template <typename Tr>
class Read {
	typedef typename Tr::N N;
	typedef typename Tr::E E;

	ifstream thefile;
	string filename;

	graph* theGraph;

	public:
		Read(string file) : filename(file){
			string line;
			int nodes, dir, weight;
			N node_name, vi, vf;

			thefile.open(filename);
            if (!thefile.is_open()) throw "Could't open the file";
            
            thefile >> nodes;
            thefile >> dir;

            thefile.ignore(256, '\n');
            streampos bookmark = thefile.tellg();
            getline(thefile, line, '\n'); // read third line

            thefile.seekg(bookmark);

            if (line.find(" ")==string::npos){ // not found
            	theGraph = new graph;
            	for (int i=0; i<nodes;++i){
            		thefile >> node_name;
            		theGraph->add_node(node_name);
            	}
            }
            else {theGraph = new graph(nodes);}

            while (thefile >> vi >> vf >> weight){
            	theGraph->add_edge(vi, vf, weight, dir);
            }

            thefile.close();
		}

		graph getGraph() {
            return *theGraph;
        }
};

typedef Read<Traits> read;

#endif
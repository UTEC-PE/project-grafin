//#include <GL/glut.h>
#include <iostream>

#include "graph.h"

using namespace std;

int main(int argc, char *argv[]) {
   graph mygraph(11);
   mygraph.add_edge(4,2,10,0);


    return EXIT_SUCCESS;
}
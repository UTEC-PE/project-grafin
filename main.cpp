#include <iostream>

#include "graph.h"

using namespace std;

int main(int argc, char *argv[]) {
   graph mygraph(11);
   mygraph.add_edge(4,2,10,0);
   mygraph.print();

   cout << "\n\nNumber of nodes: " << mygraph.size()[0];
   cout << "\nNumber of edges: " << mygraph.size()[1];

    return EXIT_SUCCESS;
}
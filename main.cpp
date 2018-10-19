#include <iostream>

#include "graph.h"

using namespace std;

int main(int argc, char *argv[]) {
   graph mygraph(11);
   mygraph.add_edge(4,6,10,0);
   mygraph.add_edge(4,7,10,0);
   mygraph.add_edge(4,3,10,0);
   mygraph.print();

   cout << "\n\nNumber of nodes: " << mygraph.size()[0];
   cout << "\nNumber of edges: " << mygraph.size()[1];


   graph mygraph2(8);
   mygraph2.add_edge(0,2,9,0);
   mygraph2.add_edge(0,1,9,0);
   mygraph2.add_edge(1,3,11,0);
   mygraph2.add_edge(1,4,9,0);
   mygraph2.add_edge(2,6,7,0);
   mygraph2.add_edge(5,2,7,0);
   mygraph2.add_edge(6,7,7,0);

   cout<<"Print: "<<endl;
   mygraph2.print();
   cout<<"BFS"<<endl;
   mygraph2.BFS(0);


    return EXIT_SUCCESS;
}
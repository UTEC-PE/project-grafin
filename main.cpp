#include <iostream>
#include <list>
#include "graph.h"

using namespace std;

int main(int argc, char *argv[]) {
   graph mygraph(11);
   mygraph.add_edge(4,6,10,1);
   mygraph.add_edge(4,7,10,1);
   mygraph.add_edge(4,3,10,1);
   mygraph.add_edge(6,7,10,1); //Esta arista completa un ciclo
   mygraph.print();
   mygraph.print_degrees();

   cout << "\n\nNumber of nodes: " << mygraph.size()[0];
   cout << "\nNumber of edges: " << mygraph.size()[1];
   cout << "\nDensity: " << mygraph.density();
   cout << "\nisDense (0.2): " << mygraph.is_dense(0.2);

   cout << "\n\nMinimum spanning tree";
   graph minimalTree = mygraph.kruskalAlgorithm();
   minimalTree.print();
   cout << "\n\nNumber of nodes: " << minimalTree.size()[0];
   cout << "\nNumber of edges: " << minimalTree.size()[1]<<endl;


   cout<<"ejemplo2: "<<endl;
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
   cout<<endl<<"DFS: "<<endl;
   mygraph2.DFS(0);
   cout<<"conexo: "<<mygraph2.isconexo()<<endl;
   cout<<"fuertemente conexo:"<<mygraph2.is_fuertemente_conexo();



    return EXIT_SUCCESS;
}
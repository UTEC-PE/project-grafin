#include <iostream>
#include <list>
#include "graph.h"

using namespace std;

int main(int argc, char *argv[]) {
   /*
   cout << "\n\n------------- Graph test 1 (Directed) -------------\n";
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


   cout << "\n\n------------- Graph test 2 (Undirected) -------------\n";
   graph mygraph2(8);
   mygraph2.add_edge(0,2,9,0);
   mygraph2.add_edge(0,1,9,0);
   mygraph2.add_edge(1,3,11,0);
   mygraph2.add_edge(1,4,9,0);
   mygraph2.add_edge(2,6,7,0);
   mygraph2.add_edge(5,2,7,0);
   mygraph2.add_edge(6,7,7,0);
   mygraph2.print();

   cout<<"\n\nDFS (desde nodo 0)";
   vector <int> showlist;
   showlist=mygraph2.ChangeNododirToData(mygraph2.DFS(0));
   for(int i=0;i<showlist.size();++i){
      cout<<showlist[i]<<" - ";
   }

   cout<<"\nConexo: "<<mygraph2.isconexo()<<endl;
   cout<<"\nFuertemente conexo: "<<mygraph2.is_fuertemente_conexo();
   cout<<"\nFuertemente conexo: "<<mygraph2.is_fuertemente_conexo();
   cout<<"\n\nBFS";

   showlist.empty();
   showlist=mygraph2.ChangeNododirToData(mygraph2.BFS(0));
   for(int i=0;i<showlist.size();++i){
      cout<<showlist[i]<<" - ";
   }
*/

   cout << "------------- Graph test 3 (Strings) -------------\n";
   graph mygraph(11);
   mygraph.add_edge("D","F",10,1);
   mygraph.add_edge("D","G",10,1);
   mygraph.add_edge("D","C",10,1);
   mygraph.add_edge("F","G",10,1); //Esta arista completa un ciclo
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

   cout<<"\n\nDFS (desde nodo A): ";
   vector <string> showlist;
   showlist=mygraph.DFS("A");
   for(int i=0;i<showlist.size();++i){
      cout<<showlist[i]<<" - ";
   }

   showlist.empty();
   cout<<"\n\nBFS (desde nodo A): ";
   showlist=mygraph.BFS("A");
   for(int i=0;i<showlist.size();++i){
      cout<<showlist[i]<<" - ";
   }

   cout<<"\n\nConexo: "<<mygraph.isconexo()<<endl;
   cout<<"\nFuertemente conexo (0 undirected) : "<<mygraph.is_fuertemente_conexo();
   cout<<"\nCompleto: "<<mygraph.is_completo();


    return EXIT_SUCCESS;
}


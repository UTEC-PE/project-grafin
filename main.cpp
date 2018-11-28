#include <iostream>
#include <list>
#include "graph.h"
#include "read.h"

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

    graph mygraph3(7);
    mygraph3.add_edge("A","D",9,0);
    mygraph3.add_edge("D","E",4,0);
    mygraph3.add_edge("D","C",4,0);
    mygraph3.add_edge("D","G",2,0);
    mygraph3.add_edge("G","C",5,0);
    mygraph3.add_edge("C","F",6,0);
    mygraph3.add_edge("B","F",2,0);
    mygraph3.add_edge("B","E",3,0);
    mygraph3.add_edge("B","C",4,0);
    mygraph3.PrimAlgorithm("A",1);
    mygraph3.PrimAlgorithm("A",1).print();


*/


    cout << "------------- Graph test 4 (Dijkstra y GreedyBFS) -------------\n";
/*
    graph dij(8);
    dij.add_edge(0,2,4,0);
    dij.add_edge(0,5,7,0);
    dij.add_edge(2,3,3,0);
    dij.add_edge(2,5,2,0);
    dij.add_edge(5,6,8,0);
    dij.add_edge(2,6,9,0);
    dij.add_edge(3,4,3,0);
    dij.add_edge(3,6,7,0);
    dij.add_edge(6,4,2,0);
    dij.add_edge(6,7,3,0);
    dij.add_edge(4,7,7,0);
    dij.add_edge(7,1,3,0);


    // dij.add_edge(4,1,9,0);
    dij.Dijkstra(0,1);

    dij.add_edge(4,1,4,0);
*/
    // dij.GreedyBFS(0, 2,1);
 /*
    cout << "------------- Graph test 5 (Floyd Warshall) -------------\n";

    graph floyd(5);
    floyd.add_edge(0,3,3,1);
    floyd.add_edge(0,2,6,1);
    floyd.add_edge(1,0,3,1);
    floyd.add_edge(2,3,2,1);
    floyd.add_edge(3,2,1,1);
    floyd.add_edge(3,1,1,1);
    floyd.add_edge(4,1,4,1);
    floyd.add_edge(4,3,2,1);

    floyd.print();
    auto pair = floyd.floydWarshall();
    pair.first.print();
    pair.second.print();

    // https://www.geeksforgeeks.org/wp-content/uploads/Fig-11.jpg
    graph dij(9);
    dij.add_edge(0,1,4,0);
    dij.add_edge(0,7,8,0);
    dij.add_edge(1,7,11,0);
    dij.add_edge(1,2,8,0);
    dij.add_edge(7,8,7,0);
    dij.add_edge(7,6,1,0);
    dij.add_edge(8,2,2,0);
    dij.add_edge(8,6,6,0);
    dij.add_edge(2,5,4,0);
    dij.add_edge(6,5,2,0);
    dij.add_edge(2,3,7,0);
    dij.add_edge(3,5,14,0);
    dij.add_edge(5,4,10,0);
    dij.add_edge(3,4,9,0);


    // https://www.youtube.com/watch?v=obWXjtg0L64
    graph bellman(6);
    bellman.add_edge("F", "E", 8, 1);
    bellman.add_edge("F", "A", 10, 1);
    bellman.add_edge("A", "C", 2, 1);
    bellman.add_edge("B", "A", 1, 1);
    bellman.add_edge("C", "B", -2, 1);
    bellman.add_edge("D", "C", -1, 1);
    bellman.add_edge("D", "A", -4, 1);
    bellman.add_edge("E", "D", 1, 1);
    bellman.add_edge("A", "C", 2, 1);
    bellman.print();
    bellman.bellmanFord("F");

    graph bellman(5);
    bellman.add_edge("A", "B", -1, 1);
    bellman.add_edge("A", "C", 4, 1);
    bellman.add_edge("B", "C", 3, 1);
    bellman.add_edge("B", "D", 2, 1);
    bellman.add_edge("B", "E", 2, 1);
    bellman.add_edge("A", "C", 4, 1);
    bellman.add_edge("D", "B", 1, 1);
    bellman.add_edge("A", "C", 4, 1);
    bellman.add_edge("D", "C", 5, 1);
    bellman.add_edge("E", "D", 3, 1);

    bellman.print();
    bellman.bellmanFord("F", 1);
*/
    /*
    read readGraph1("txt_graphs/test_int.txt");
    graph test1 = readGraph1.getGraph();

    cout << "\nfloydWarshall:\n";
    auto pair = test1.floydWarshall();
    pair.first.print();
    cout <<endl;
    pair.second.print();

    cout << "\nbellmanFord:\n";
    test1.bellmanFord(0);

    cout << "\nDijkstra:\n";
    test1.Dijkstra(0, true);

    cout << "\nGreedyBFS:\n";
    test1.GreedyBFS(0,2, true);
    */

    graph Graph;

    Graph.add_node(1, 0, 0);
    Graph.add_node(2, 1, 2);
    Graph.add_node(3, 1, 0);
    Graph.add_node(4, 1, -2);
    Graph.add_node(5, 3, 2);
    Graph.add_node(6, 3, 0);
    Graph.add_node(7, 3, -2);
    Graph.add_node(8, 5, -2);
    Graph.add_node(9, 5, 0);

    Graph.add_edge(1, 2, 5, 1);
    Graph.add_edge(1, 3, 25, 1);
    Graph.add_edge(2, 5, 11, 1);
    Graph.add_edge(2, 6, 33, 1);
    Graph.add_edge(3, 4, 17, 1);
    Graph.add_edge(3, 6, 38, 1);
    Graph.add_edge(4, 7, 12, 1);
    Graph.add_edge(5, 6, 21, 1);
    Graph.add_edge(5, 9, 1, 1);
    Graph.add_edge(6, 4, 4, 1);
    Graph.add_edge(6, 7, 3, 1);
    Graph.add_edge(7, 8, 50, 1);
    Graph.add_edge(8, 6, 41, 1);
    Graph.add_edge(9, 6, 2, 1);
    Graph.add_edge(9, 8, 14, 1);

    // read readGraph1("txt_graphs/test_int.txt");
    // graph test1 = readGraph1.getGraph();

    cout << "\nfloydWarshall:\n";
    auto pair = Graph.floydWarshall();
    pair.first.print();
    cout <<endl;
    pair.second.print();

    cout << "\nbellmanFord:\n";
    Graph.bellmanFord(1, true);

    cout << "\nDijkstra:\n";
    Graph.Dijkstra(1, true);

    cout << "\nGreedyBFS:\n";
    Graph.GreedyBFS(1,4, true);


    return EXIT_SUCCESS;
}


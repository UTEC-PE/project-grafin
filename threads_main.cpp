#include <iostream>
#include <utility>
#include <thread>

#include "graph.h"
#include "read.h"

using namespace std;

vector<graph> results;

template <typename P>
void parallel_A(graph thegraph, P node_begin, P node_end){
    results.push_back(thegraph.A_asterisco(node_begin, node_end, true));
}

// void hello(){
//     cout << "Hello";
// }

int main(int argc, char *argv[]) {
    int const num_threads = 3;

    // read readGraph1("txt_graphs/graph3_int.txt");
    // graph test1 = readGraph1.getGraph();

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



    vector<pair<int, int>> search;

    cout << "\nIngrese nodos a buscar con A*";
    cout << "\nEjm: 0 1 significa del nodo 0 al 1\n> ";
    int node1, node2;
    while ( cin >>  node1 >> node2 ){
        search.push_back(make_pair(node1, node2));
    }

    thread threads[num_threads];

    for (int i=0; i<search.size(); ++i){
        threads[i%num_threads] = thread(parallel_A, Graph, search[i].first, search[i].second);
        if (!i%num_threads) for (int i = 0; i <num_threads; ++i) threads[i].join();
    }

    for (auto& theresult: results) theresult.print();



    return EXIT_SUCCESS;
}


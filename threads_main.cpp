#include <iostream>
#include <utility>
#include <thread>

#include "graph.h"
#include "read.h"

using namespace std;

vector<graph> results;

template <typename P>
void parallel_A(graph thegraph, P node_begin, P node_end){
    results.push_back(thegraph.Dijkstra(node_begin, true));
}

// void hello(){
//     cout << "Hello";
// }

int main(int argc, char *argv[]) {
    int const num_threads = 3;

    read readGraph1("txt_graphs/graph3_int.txt");
    graph test1 = readGraph1.getGraph();

    vector<pair<int, int>> search;

    cout << "\nIngrese nodos a buscar con A*";
    cout << "\nEjm: 0 1 significa del nodo 0 al 1";
    int node1, node2;
    while ( cin >>  node1 >> node2 ){
        search.push_back(make_pair(node1, node2));
    }

    thread threads[num_threads];

    for (int i=0; i<search.size(); ++i){
        threads[i%num_threads] = thread(hello);
        if (!i%num_threads) for (int i = 0; i <num_threads; ++i) threads[i].join();
    }

    // for (auto& theresult: results) theresult.print();



    return EXIT_SUCCESS;
}


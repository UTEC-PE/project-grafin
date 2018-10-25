#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>
#include <algorithm>
#include <stack>
#include <iostream>
#include <queue>
#include <map>
#include <unordered_map>

#include "node.h"
#include "edge.h"

using namespace std;

class Traits {
	public:
		typedef char N;
		typedef int E;
};

struct setSizes {
	int set, size=0;
	setSizes();
	setSizes(int se) : set(se){};
	setSizes operator=(const int someset){
		set = someset; ++size;
		return *this;
	}
	bool operator!=(const int other_set){
		return set!= other_set;
	}
	ostream &operator<<(ostream &output){
		output << this->set;
		return output;
	}
};

template <typename Tr>
struct disjointSet {
	typedef typename Tr::N N;
    typedef typename Tr::E E;
	unordered_map<N, int> nodename_to_id;
	unordered_map<int, N> id_to_nodename;
	vector<setSizes> sets;

	void makeSetFor(N node_name){
		nodename_to_id.insert(pair<N, int> (node_name, sets.size()));
		id_to_nodename.insert(pair<int, N> (sets.size(), node_name));
		sets.push_back(setSizes(sets.size()));
	}

	N parentOf(N node_name){
		while (int (sets[nodename_to_id[node_name]] != nodename_to_id[node_name])){
			sets[nodename_to_id[node_name]] = sets[sets[nodename_to_id[node_name]].set];
			node_name = id_to_nodename[sets[nodename_to_id[node_name]].set];
		}
		return node_name;
	}

	void unionOf(N node_name1, N node_name2){
		int id_of_node1 = nodename_to_id[parentOf(node_name1)];
		int id_of_node2 = nodename_to_id[parentOf(node_name2)];

		if (sets[id_of_node1].size > sets[id_of_node2].size){
			sets[id_of_node2] = sets[id_of_node1].set;
		}
		else sets[id_of_node1] = sets[id_of_node2].set;
	}

	int inSameSet(N node_name1, N node_name2){ return parentOf(node_name1)==parentOf(node_name2);}

	// debugging
	void print(){
		cout <<"\nCycles";
		for (auto&  node: nodename_to_id) cout << "\nNode: " << node.first << "\tSet: " << sets[node.second].set << "\tParent: " << node.second;
	}
};

typedef disjointSet<Traits> DisjointSet;

template <typename Tr>
class Graph {
    public:
    	typedef typename Tr::N N;
        typedef typename Tr::E E;
        typedef Graph<Tr> self;
        typedef Node<self> node;
        typedef Edge<self> edge;
        typedef map<N, node*> NodeSeq;
        typedef list<node*> NodeList;
        typedef list<edge*> EdgeSeq;
				//Renombrar iteradores
        typedef typename NodeSeq::iterator NodeIte;
        typedef typename EdgeSeq::iterator EdgeIte;

    private:
        NodeSeq nodes; //Mapa de punteros de nodos <data (node name), node pointer>
					   //Iteradores
        NodeIte ni;
        EdgeIte ei;
        bool has_direction=false;
        int sizeOfGraph[2]= {0,0}; // sizeOfGraph[0]: # de nodes							// sizeOfGraph[1]: # de edges
        void add_edge(edge someedge){
        	add_edge(someedge.nodes[0]->get_data(), someedge.nodes[1]->get_data(), someedge.get_peso(), someedge.get_dir());
        }

    //TODO: Es bueno que un constructor este en private?
    Graph(NodeSeq somenodes){ // Nuevo grafo a partir de data de vector de nodos
    	node* newnode;
    	for (auto pairNodes : somenodes){
    		add_node(pairNodes.first);
        }
    };
	public:
	Graph(int size) : Graph(size, is_arithmetic<N>{}){}

	Graph(int size, true_type) { // int, float, char
		sizeOfGraph[0] = size;
		node* newnode;
		for (N i=0;i<size;++i){
			newnode=new node(i+65*(sizeof(N)==1));
			nodes.insert(pair <N, node*> (i+65*(sizeof(N)==1), newnode));
		}
	}
	Graph(int size, false_type) { // string
		sizeOfGraph[0] = size;
		node* newnode;
		for (char i=0;i<size;++i){
			newnode=new node(string(1, i+65));
			nodes.insert(pair <N, node*> (string(1, i+65), newnode));
		}
	}

	Graph(const Graph &other_graph) { // copy constructor
		nodes = other_graph.nodes;
		has_direction= other_graph.has_direction;
		sizeOfGraph[0] = other_graph.sizeOfGraph[0];
		sizeOfGraph[1] = other_graph.sizeOfGraph[1];
	}
	Graph(){};

	bool add_node(N node_name){
		if (nodes.find(node_name)!=nodes.end()) return false; // name taken
		node* newnode = new node(node_name);
		nodes.insert(pair<N, node*> (node_name, newnode));
		++sizeOfGraph[0];
		return true;
	};

	float density(){
		return (!has_direction+1)*float(sizeOfGraph[1])/(float(sizeOfGraph[0])*(float(sizeOfGraph[0]-1)));
	}

	bool is_dense(float cota){
		if (0>=cota>=1) throw "La cota debe estar entre 0 y 1";
		return cota<this->density();
	}

	// bool add_node(N data){
	// 	if (find(nodes.begin(), nodes.end(), data)) return false; //el nombre ya está tomado
		//TODO: Mantener el vector de nodos ordenado
		//		Convertir el vector de nodos a map
		//		Aceptar un add_node() sin parámetros que continue la secuencia de nodos
	//}


	bool add_edge(N Vi, N Vf, E peso, bool dir, bool recursive=false){
		//TODO: Crear subclases de grafo para dirigido y no dirigido
		has_direction = dir;

		// comprobar que los vertices existan
		if (!(nodes.count(Vi) && nodes.count(Vf)))
			return false;

		node* initial_node=nodes[Vi];
		node* final_node=nodes[Vf];

		// si el edge no tiene direccion, no hay nodo final ni inicial
		// (debe agregarse en la lista de edges de ambos nodos)
		if (!dir && !recursive) {
            add_edge(Vf, Vi, peso, false, true);
        }

		edge* new_edge = new edge(initial_node,final_node,peso,dir);

		auto edge_in_edges = initial_node->edges.begin();

		// para mantener los edges ordenados en node.edges
		while (edge_in_edges!=initial_node->edges.end() && *new_edge>**edge_in_edges) ++edge_in_edges;

		if (initial_node->edges.empty() || *edge_in_edges==*initial_node->edges.end()) initial_node->edges.push_back(new_edge); // el nuevo edge debe ir al final
		else if (*new_edge==**edge_in_edges) return false; 					// hay otro edge con un mismo inicio y fin
		else initial_node->edges.insert(edge_in_edges, new_edge);

		// aumentar grado
		++initial_node->gradoEntrada;
		++final_node->gradoSalida;

		// si se esta agregando un edge compartido (sin direccion), no aumentar
		sizeOfGraph[1]+=1*!recursive;
		return true;
	}

	int get_tipo(N name){
		return nodes[name]->get_tipo();
	}

	int* size(){ return sizeOfGraph; }

	// Solo para debugging
	void print(){
		for (auto& thenode : nodes){
			cout<< "\nNodo " << thenode.first << ": ";
			for (auto& theedge : thenode.second->edges){
				cout << theedge->nodes[1]->get_data() << " ";
			}
		}
	}

	void print_degrees(){
		cout <<"\n\n\t\t\tGE" <<"\tGS"<<"\tTipo";
		for (auto& thenode : nodes){
			cout<< "\nNodo " << thenode.first << ": ";
			cout<<"\t"<<thenode.second->gradoEntrada;
			cout<<"\t"<<thenode.second->gradoSalida;
			switch(thenode.second->get_tipo()){
				case 0:  cout<<"\tFuente"; break;
				case 1:  cout<<"\tHundido"; break;
				case 2:  cout<<"\tNada";
			}
		}
	}

	self kruskalAlgorithm(){
		vector<edge> sortedEdges;
		DisjointSet cycles;
		self minimalTree(nodes);

		//Ordenar edges por peso
		for (auto& thenode : nodes){
			cycles.makeSetFor(thenode.first); // Inicializar disjoint set
			for (auto& theedge : thenode.second->edges){
				sortedEdges.push_back(*theedge);
			}
		}
		sort(sortedEdges.begin(), sortedEdges.end());

		for (edge theedge : sortedEdges){
			// Si no se forman ciclos, agregar el edge y agregar nodos al disjoint set
			if ( !cycles.inSameSet(theedge.nodes[0]->get_data(), theedge.nodes[1]->get_data()) ){
				cycles.unionOf(theedge.nodes[0]->get_data(), theedge.nodes[1]->get_data());
				minimalTree.add_edge(theedge);
			}
		}
		return minimalTree;
	}

	//busqueda por profundidad
    NodeList DFS_nodes(N nodo_data_inicial){
		node* nodo_inicial= nodes[nodo_data_inicial];
	    node* actual;
		stack<node*> pila_stack;
		list <node*> lista;

		pila_stack.push(nodo_inicial);
		//cuado el stack esta vacio ya no habra mas nodos por visitar
		while(!pila_stack.empty()){
			bool thereis=false;
			actual= pila_stack.top();
			pila_stack.pop();


			//recoremos todas las aristas del nodo actual para ver si ya esta en la lista de visitados
			for(auto it=lista.begin(); it!=lista.end();++it){
				if(*it==actual){
					thereis=true;
					break;
				}
			}
			//si no encontramos la arista, la agregamos a la lista de visitados, la imprimimos y la recorremos
			if(!thereis){
				lista.push_back(actual);
					//cout  << actual->get_data() << " -> ";
				edge *auxedge;

				//recoremos las aristas
					for (auto it2=actual->edges.begin(); it2!=actual->edges.end(); it2++){
						auxedge=(*it2);
						bool thereis2=false;

						for(auto it=lista.begin(); it!=lista.end();it++){
                            if((*it)==auxedge->nodes[1]){
								thereis2=true;
							}

						}
						if(!thereis2) {
							pila_stack.push(auxedge->nodes[1]);
						}
					}

			}
		}
		return lista;
	}
//no dir
	bool isconexo(){
        if(has_direction) return false;
        return sizeOfGraph[0] == DFS_nodes (nodes.begin()->first).size();

        /*
        for(auto it=nodes.begin(); it!=nodes.end(); ++it){
            if((DFS_nodes((*it)->get_data())).size() == nodes.size()) return true;
        }
         */
    }
        
    bool is_completo(){
    	for (auto& thenode : nodes){
    		if(thenode.second->gradoEntrada != (sizeOfGraph[0] -1)) return false;
    	}
    	return true;
	}

	//dirc
	bool is_fuertemente_conexo(){
        if(!has_direction) return false;
        for (auto& thenode : nodes){
    		if ( DFS_nodes(thenode.first).size() != sizeOfGraph[0]) return false;
    	}
    	return true;
    }

	NodeList BFS_nodes(N nodo_data_inicial){
	    node* nodo_inicial=nodes[nodo_data_inicial];
	    list<node*> lista;
	    queue<node*> queue_cola;
        queue_cola.push(nodo_inicial);
        node* actual;
	    while(!queue_cola.empty()){
	        actual=queue_cola.front();
	        bool thereis=false;
	        queue_cola.pop();

	        for(auto it=lista.begin();it!=lista.end();++it){
	            if(*it ==actual){
	                thereis=true;
                    break;
	            }
	        }

	        if(!thereis){
	            //cout<<actual->get_data()<<" -> ";
	            lista.push_back(actual);
                edge* auxedge;
	            for(auto it=actual->edges.begin();it!=actual->edges.end();++it){
	                auxedge=(*it);
	                bool thereis2=false;
                    for(auto it2=lista.begin();it2!=lista.end();++it2){
                        if((*it2)==auxedge->nodes[1]){
                            thereis2=true;
                            break;
                        }
                    }
                    if(!thereis2){
                        queue_cola.push(auxedge->nodes[1]);
                    }
	            }
	        }
	    }
	    return lista;
	}

    vector<N> BFS(N nodo_data_inicial){
		if (!(nodes.count(nodo_data_inicial)))throw "Nodo no existe";
		else {return ChangeNododirToData(BFS_nodes(nodo_data_inicial));}
	}
    vector<N> DFS(N nodo_data_inicial){
		if (!(nodes.count(nodo_data_inicial)))throw "Nodo no existe";
		else {return ChangeNododirToData(DFS_nodes(nodo_data_inicial));}
    }
	vector<N> ChangeNododirToData(list<node*> lista_directions){
	    vector<N> nueva_lista;
	    for (auto& element : lista_directions){
	    	nueva_lista.push_back(element->get_data());
	    }
	    return nueva_lista;
	}
};

typedef Graph<Traits> graph;

#endif
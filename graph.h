#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>
#include <algorithm> 
#include <numeric>
#include <stack>
#include <iostream>
#include <queue>

#include "node.h"
#include "edge.h"

using namespace std;

class Traits {
	public:
		typedef int N;
		typedef int E;
};
template <typename Tr>
class Graph {
    public:
        typedef Graph<Tr> self;
        typedef Node<self> node;
        typedef Edge<self> edge;
        typedef vector<node*> NodeSeq;
        typedef list<node*> NodeList;
        typedef list<edge*> EdgeSeq;
        typedef typename Tr::N N;
        typedef typename Tr::E E;
				//Renombrar iteradores
        typedef typename NodeSeq::iterator NodeIte;
        typedef typename EdgeSeq::iterator EdgeIte;

    private:
        NodeSeq nodes; //Vector de punteros de nodos
					   //Iteradores
        NodeIte ni;
        EdgeIte ei;
        bool has_direction=false;
        int sizeOfGraph[2]= {0,0}; // sizeOfGraph[0]: # de nodes							// sizeOfGraph[1]: # de edges
        void add_edge(edge someedge){
        	add_edge(someedge.nodes[0]->get_data(), someedge.nodes[1]->get_data(), 
        		someedge.get_peso(), someedge.get_dir());
        }

    //TODO: Es bueno que un constructor este en private?
    Graph(NodeSeq somenodes){ // Nuevo grafo a partir de data de vector de nodos
    	node* newnode;
    	for (node* pnodes : somenodes){
        		newnode = new node(pnodes->get_data());
        		nodes.push_back(newnode);
        		++sizeOfGraph[0];
        	}
    };
	public:
	Graph(int size) {
		sizeOfGraph[0] = size;
		node* newnode;
		for (int i=0;i<size;++i){
			newnode=new node(i);
			nodes.push_back(newnode);
		}
	}
	Graph(){};

	float density(){
		return (!has_direction+1)*float(sizeOfGraph[1])/(float(sizeOfGraph[0])*(float(sizeOfGraph[0]-1)));
	}

	bool is_dense(float cota){
		if (0>=cota>=1) throw "La cota debe estar entre 0 y 1";
		return cota<this->density();
	}

	bool add_node(N data){
		if (find(nodes.begin(), nodes.end(), data)) return false; //el nombre ya está tomado
		//TODO: Mantener el vector de nodos ordenado
		//		Convertir el vector de nodos a map
		//		Aceptar un add_node() sin parámetros que continue la secuencia de nodos
	}


	bool add_edge(int Vi, int Vf, E peso, bool dir, bool recursive=false){
		//TODO: Crear subclases de grafo para dirigido y no dirigido
		has_direction = dir;

		node* initial_node=nodes.at(Vi);
		node* final_node=nodes.at(Vf);

		// comprobar que los vertices existan
		if (!(initial_node && final_node))
			return false;

		// si el edge no tiene direccion, no hay nodo final ni inicial
		// (debe agregarse en la lista de edges de ambos nodos)
		if (!dir && !recursive) {
            add_edge(Vf, Vi, peso, false, true);
        }

		edge* new_edge = new edge(initial_node,nodes[Vf],peso,dir);

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
		for (int i=0; i<nodes.size(); ++i){
			cout<< "\nNodo " << i << ": ";
			for (auto it=nodes[i]->edges.begin(); it!=nodes[i]->edges.end(); it++){
				cout << (*it)->nodes[1]->get_data() << " ";
			}
		}
	}

	void print_degrees(){
		cout <<"\n\t\t\tGE" <<"\tGS"<<"\tTipo";
		for (int i=0; i<nodes.size(); ++i){
			cout<< "\nNodo " << i << ":";
			cout<<"\t\t"<<nodes[i]->gradoEntrada;
			cout<<"\t"<<nodes[i]->gradoSalida;
			switch(nodes[i]->get_tipo()){
				case 0:  cout<<"\tFuente"; break;
				case 1:  cout<<"\tHundido"; break;
				case 2:  cout<<"\tNada";
			}
		}
	}

	self kruskalAlgorithm(){
		vector<edge> sortedEdges;
		//Iterate through array
		//	Iterate through edges
		//	Add edges (not pointers)
		for (auto it=nodes.begin(); it!=nodes.end(); ++it)
			for (auto edgeit=(*it)->edges.begin(); edgeit!=(*it)->edges.end(); ++edgeit){
				sortedEdges.push_back(**edgeit);
			}
		sort(sortedEdges.begin(), sortedEdges.end());

		vector<N> disjointSet(nodes.size());
		iota(disjointSet.begin(), disjointSet.end(), 0); // Make sets (iota rellena con secuencia)
		self minimalTree(this->nodes);

		// TODO: Hacer que disjointSet funcione con ids y no solo si la posición
		//		 del nodo es igual a su data
		for (edge theedge : sortedEdges){
			int disjointElementSet0 = disjointSet[theedge.nodes[0]->get_data()];
			int disjointElementSet1 = disjointSet[theedge.nodes[1]->get_data()];

			if (disjointElementSet0!=disjointElementSet1){ // Find
				minimalTree.add_edge(theedge);

				// TODO: Tratar de encontrar una manera menos 'verbosa'
				if (disjointElementSet0<disjointElementSet1) // Union (el mayor set se mergea al menor)
					disjointSet[disjointElementSet1]=disjointElementSet0;
				else
					disjointSet[disjointElementSet0]=disjointElementSet1;
			}
		}
		return minimalTree;
	}

	//busqueda por profundidad
    NodeList DFS(int nodo_data_inicial){
		node* nodo_inicial= nodes.at(nodo_data_inicial);
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

	bool isconexo(){
        if(!has_direction) {
            return (nodes.size() == (DFS (nodes[0]->get_data())).size());
        }
        else{
            for(auto it=nodes.begin(); it!=nodes.end(); ++it){
                if((DFS((*it)->get_data())).size() == nodes.size()) return true;
            }
            return false;
        }
	}

	bool is_fuertemente_conexo(){
        if(!has_direction) {
            return (nodes.size() == (DFS (nodes[0]->get_data())).size());
        }
        else{
            for(auto it=nodes.begin(); it!=nodes.end(); ++it){
                if((DFS((*it)->get_data())).size() != nodes.size()) return false;
            }
            return true;
        }
    }

	NodeList BFS(int dataof,bool return_lista_int=true){
	    node* nodo_inicial=nodes.at(dataof);
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

	vector<int> ChangeNododirToData(list<node*> lista_directions){
	    vector<int> nueva_lista;
	    for(auto it=lista_directions.begin();it!=lista_directions.end();++it){
	        nueva_lista.push_back((int) (*it)->get_data());
	    }
	    return nueva_lista;
	}
};

typedef Graph<Traits> graph;

#endif
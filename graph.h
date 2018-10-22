#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>
#include <algorithm>
#include <stack>
#include <iostream>
#include <queue>
#include <map>

#include "node.h"
#include "edge.h"

using namespace std;

class Traits {
	public:
		typedef string N;
		typedef int E;
};

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
        		newnode = new node(pairNodes.first);
        		nodes.insert(pair <N, node*> (pairNodes.first, newnode));
        		++sizeOfGraph[0];
        	}
    };
	public:
	Graph(int size) : Graph(size, is_arithmetic<N>{}){}

	Graph(int size, true_type) { // int, float, char
		sizeOfGraph[0] = size;
		node* newnode;
		N i=65*(sizeof(N)==1);
		for (;i<size;++i){
			newnode=new node(i);
			nodes.insert(pair <N, node*> (i, newnode));
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
	Graph(){};

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
		//Iterate through array
		//	Iterate through edges
		//	Add edges (not pointers)
		for (auto& thenode : nodes)
			for (auto& theedge : thenode.second->edges){
				sortedEdges.push_back(*theedge);
			}

		sort(sortedEdges.begin(), sortedEdges.end());

		map<N, int> disjointSet;
		int c=-1;
		for (auto& thenode : nodes) disjointSet.insert(pair<N, int> (thenode.first, ++c));
		self minimalTree(this->nodes);

		// TODO: Hacer que disjointSet funcione con ids y no solo si la posición
		//		 del nodo es igual a su data
		for (edge theedge : sortedEdges){
			N disjointKey0 = theedge.nodes[0]->get_data();
			N disjointKey1 = theedge.nodes[1]->get_data();

			if (disjointSet[disjointKey0]!=disjointSet[disjointKey1]){ // Find
				minimalTree.add_edge(theedge);

				// TODO: Tratar de encontrar una manera menos 'verbosa'
				if (disjointSet[disjointKey0]<disjointSet[disjointKey1]) // Union (el mayor set se mergea al menor)
					disjointSet[disjointKey1]=disjointSet[disjointKey0];
				else
					disjointSet[disjointKey0]=disjointSet[disjointKey1];
			}
		}
		return minimalTree;
	}

    self PrimAlgorithm(N dataof, bool primprint=false){
        list <edge*> lista_edges;
        multimap< E ,edge*> edge_map;
        list <node*> nodos_visitados;
        Graph PrimGraph(nodes.size());

        nodos_visitados.push_back(nodes[dataof]);
        for(auto it3=nodes[dataof]->edges.begin();it3!=nodes[dataof]->edges.end();++it3){
            edge_map.insert(pair <E, edge*>((*it3)->get_peso(), *it3));
            //cout<<(*it3)->nodes[1]->get_data()<<"| "<<(*it3)->get_peso()<<endl;
        }

        while(nodos_visitados.size()!=nodes.size()){
            auto it=edge_map.begin();
            edge* aux_edge=it->second;
            //cout<<"maybe next edge pair : "<<aux_edge->nodes[0]->get_data()<<","<<aux_edge->nodes[1]->get_data()<<"||"<<endl;
            bool thereis=false;
            for(auto it2=nodos_visitados.begin();it2!=nodos_visitados.end();++it2){
                if(aux_edge->nodes[1]==*it2){
                    //si la prox arista tiene en nodes[1] un nodo que ya vsito, lo borra
                    //cout<<"erase"<<endl;
                    edge_map.erase(edge_map.begin());
                    thereis=true;
                    break;
                }
            }
            if(thereis==false){
                //cout<<"yes"<<endl;
                //borro el 1ero del multimapa antes de agregarle los nuevos edges
                edge_map.erase(edge_map.begin());
                if(primprint) { lista_edges.push_back(aux_edge); }
                PrimGraph.add_edge((aux_edge)->nodes[0]->get_data(),(aux_edge)->nodes[1]->get_data(),(aux_edge)->get_peso(),0);
                nodos_visitados.push_back(aux_edge->nodes[1]);
                for(auto it3=aux_edge->nodes[1]->edges.begin();it3!=aux_edge->nodes[1]->edges.end();++it3){
                    //cout<<"edges++"<<endl;
                    edge_map.insert(pair <E, edge*>( (*it3)->get_peso(), *it3));
                }
            }
        }
        /*
        cout<<"nodos visit :"<<nodos_visitados.size()<<endl;
        cout<<"lista de edges size: "<<lista_edges.size()<<endl;
        cout<<"edge map size:"<<edge_map.size()<<endl;
         */

        if(primprint){
            for(auto aux_edge=lista_edges.begin();aux_edge!=lista_edges.end();++aux_edge){
                cout<<(*aux_edge)->nodes[0]->get_data()<<","<<(*aux_edge)->nodes[1]->get_data()<<"||"<<endl;
            }
        }
        return PrimGraph;
    }



    //busqueda por profundidad
    NodeList DFS(N nodo_data_inicial){
    	if (!(nodes.count(nodo_data_inicial))) throw "Nodo no existe";

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
        return sizeOfGraph[0] == DFS (nodes.begin()->first).size();

        /*
        for(auto it=nodes.begin(); it!=nodes.end(); ++it){
            if((DFS((*it)->get_data())).size() == nodes.size()) return true;
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
    		if ( DFS(thenode.first).size() != sizeOfGraph[0]) return false;
    	}
    	return true;
    }

	NodeList BFS(N dataof, bool return_lista_int=true){
	    node* nodo_inicial=nodes[dataof];
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
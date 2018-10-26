//TODO: Crear subclases de grafo para dirigido y no dirigido
//TODO: Colocar ids a los nodos

#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <algorithm>

#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <map>
#include <unordered_map>
#include <set>

#include "node.h"
#include "edge.h"

using namespace std;

struct Traits {
	typedef int N;
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
        typedef typename NodeSeq::iterator NodeIte;
        typedef typename EdgeSeq::iterator EdgeIte;

    private:
        NodeSeq nodes; // Mapa de punteros de nodos <data (node name), node pointer>
        EdgeSeq edges_graph; // usado en fuertemente conexo
        bool has_direction=false;
        int sizeOfGraph[2]= {0,0}; // sizeOfGraph[0]: num de nodes -  sizeOfGraph[1]: num de edges

	    Graph(NodeSeq somenodes){ // Nuevo grafo a partir de data de vector de nodos
	    	node* newnode;
	    	for (auto pairNodes : somenodes){
	    		add_node(pairNodes.first);
	        }
	    };

	    // Funciones auxiliares y utiles
	    void add_edge(edge someedge){
        	add_edge(someedge.nodes[0]->get_data(), someedge.nodes[1]->get_data(), someedge.get_peso(), someedge.get_dir());
        };

        EdgeIte remove_edge(node* vi, node* vf, bool recursive=false){
        	EdgeIte it;
        	for (auto it_edge = vi->edges.begin(); it_edge!=vi->edges.end(); ++it_edge){
				if ((*it_edge)->nodes[1]==vf) {
					it = vi->edges.erase(it_edge);
					delete *it_edge;
					break;
				}
			}

			if (!has_direction && !recursive) remove_edge(vf, vi, true);
			return it;
        }

        bool recorrido_fc(){
        	return (DFS(nodes.begin()->first).size()+1 == sizeOfGraph[0]);
		};

		void MakeAllThereisFalse(){
			for(auto it=nodes.begin();it!=nodes.end();++it){
				it->second->thereis= false;
			}
		};

	public:
		// Constructores
		Graph(int size) : Graph(size, is_arithmetic<N>{}){};

		Graph(int size, true_type) { // int, float, char
			sizeOfGraph[0] = size;
			node* newnode;
			for (N i=0;i<size;++i){
				newnode=new node(i+65*(sizeof(N)==1));
				nodes.insert(pair <N, node*> (i+65*(sizeof(N)==1), newnode));
			}
		};

		Graph(int size, false_type) { // string
			sizeOfGraph[0] = size;
			node* newnode;
			for (char i=0;i<size;++i){
				newnode=new node(string(1, i+65));
				nodes.insert(pair <N, node*> (string(1, i+65), newnode));
			}
		};

		Graph(const Graph &other_graph) { // copy constructor
			nodes = other_graph.nodes;
			has_direction= other_graph.has_direction;
			sizeOfGraph[0] = other_graph.sizeOfGraph[0];
			sizeOfGraph[1] = other_graph.sizeOfGraph[1];
		};

		Graph(){};


		// Metodos para debugging
		void print(){
			for (auto& thenode : nodes){
				cout<< "\nNodo " << thenode.first << ": ";
				for (auto& theedge : thenode.second->edges){
					cout << theedge->nodes[1]->get_data() << " ";
				}
			}
		};

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
		};


		// Metodos de informacion basica
		int* size(){ return sizeOfGraph; };

		int get_tipo(N name){
			return nodes[name]->get_tipo();
		};

		

		// Metodos fundamentales
		bool add_node(N node_name){
			if (nodes.find(node_name)!=nodes.end()) return false; // name taken
			node* newnode = new node(node_name);
			nodes.insert(pair<N, node*> (node_name, newnode));
			++sizeOfGraph[0];
			return true;
		};

		bool add_edge(N Vi, N Vf, E peso, bool dir, bool recursive=false){
			has_direction = dir;

			// Comprobar que los vertices existan
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
			else if (*new_edge==**edge_in_edges) return false; // hay otro edge con un mismo inicio y fin
			else initial_node->edges.insert(edge_in_edges, new_edge);


			if(recursive==false){ edges_graph.push_back(new_edge);} //solo se agrega una vez
			// aumentar grado
			++initial_node->gradoEntrada;
			++final_node->gradoSalida;

			// si se esta agregando un edge compartido (sin direccion), no aumentar
			sizeOfGraph[1]+=1*!recursive;
			return true;
		};

		bool remove_node(N node_name){
			if (nodes.find(node_name)==nodes.end()) return false; // not found
			node* to_remove = nodes[node_name];

			auto it_edge = to_remove->edges.begin();
			while (it_edge!=to_remove->edges.end()){
				it_edge = remove_edge((*it_edge)->nodes[0], (*it_edge)->nodes[1]);
			}
			delete to_remove;
			nodes.erase(node_name);
			return true;
		}

		bool remove_edge(N vi_name, N vf_name, bool recursive=false){
			if (nodes.find(vi_name)==nodes.end() || nodes.find(vf_name)==nodes.end()) return false; // not found
			node* vi = nodes[vi_name];
			node* vf = nodes[vf_name];

			remove_edge(vi, vf);
			return true;
		}


		// Propiedades
		float density(){
			return (!has_direction+1)*float(sizeOfGraph[1])/(float(sizeOfGraph[0])*(float(sizeOfGraph[0]-1)));
		};

		bool is_dense(float cota){
			if (0>=cota>=1) throw "La cota debe estar entre 0 y 1";
			return cota<this->density();
		};

		bool isconexo(){ 	// no dirigidos
	        if(has_direction) throw "Para grafos dirigidos se usa is_fuertemente_conexo";
	        return sizeOfGraph[0] == (DFS(nodes.begin()->first, true).size()+1);
	    };

		bool is_fuertemente_conexo(){ // dirigidos
			if (!has_direction) throw "Para grafos no dirigidos se usa isconexo";
			self grafo_traspuesto(nodes);
			for (auto &newedges : edges_graph) {
				grafo_traspuesto.add_edge(newedges->nodes[1]->get_data(), newedges->nodes[0]->get_data(),
										  newedges->get_peso(), newedges->get_dir());
			}
			return (grafo_traspuesto.recorrido_fc() && (*this).recorrido_fc());
		};

		bool is_completo(){
	    	for (auto& thenode : nodes){
	    		if(thenode.second->gradoEntrada != (sizeOfGraph[0] -1)) return false;
	    	}
	    	return true;
		};

		bool bipartite(N initial_node){
			bool isbipartite=true;
			this->DFS(initial_node, true, isbipartite);

			/* For debugging
			for (auto& node: nodes){
				cout <<"Nodo "<< node.first<<"   "<<node.second->thereis<<endl;
			}
			*/
			return isbipartite;
		};

		bool bipartite(){
			return bipartite(nodes.begin()->first);
		}


		// Algoritmos en grafos 
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
		};

	    self PrimAlgorithm(N dataof, bool primprint=false){
	        multimap< E ,edge*> edge_map;
	        int nodos_visitados=0;
	        Graph PrimGraph(nodes.size());

	        ++nodos_visitados;
			nodes[dataof]->thereis=true;
	        for(auto it3=nodes[dataof]->edges.begin();it3!=nodes[dataof]->edges.end();++it3){
	            edge_map.insert(pair <E, edge*>((*it3)->get_peso(), *it3));
	            //cout<<(*it3)->nodes[1]->get_data()<<"| "<<(*it3)->get_peso()<<endl;
	        }

	        while(nodos_visitados!=nodes.size()){
	            edge* aux_edge=edge_map.begin()->second;
	            //cout<<"maybe next edge pair : "<<aux_edge->nodes[0]->get_data()<<","<<aux_edge->nodes[1]->get_data()<<"||"<<endl;
				edge_map.erase(edge_map.begin());
				if(!aux_edge->nodes[1]->thereis){
					//cout<<"si"<<endl;
					aux_edge->nodes[1]->thereis=true;
						if(primprint) {
							cout<<aux_edge->nodes[0]->get_data()<<","<<aux_edge->nodes[1]->get_data()<<"||"<<endl;
						}

					PrimGraph.add_edge((aux_edge)->nodes[0]->get_data(),(aux_edge)->nodes[1]->get_data(),(aux_edge)->get_peso(),0);
					for(auto it3=aux_edge->nodes[1]->edges.begin();it3!=aux_edge->nodes[1]->edges.end();++it3){
						//cout<<"edges++"<<endl;
						edge_map.insert(pair <E, edge*>( (*it3)->get_peso(), *it3));
					}
					++nodos_visitados;
	            }
	        }
			MakeAllThereisFalse();

			return PrimGraph;
	    };

	    void print_DFS(vector<pair<N, N>> v) {
	    	int c=0;
		    for (auto& tuple: v)
		    	cout <<"\nEdge " << ++c << ": " << tuple.first << " " << tuple.second;
		}
		
		static bool basurita;
	    vector<pair<N, N>> DFS(N nodo_data_inicial, bool silenced=false, bool &isbipartite=basurita){  // Busqueda por profundidad
	    	if (nodes.find(nodo_data_inicial)==nodes.end()) throw "Nodo no existe";
	    	
			node* actual= nodes[nodo_data_inicial];
			stack<node*> pila_stack;
			set<node*> visitados;
			vector<pair<N, N>> vector_edges_visitados;

			// agregar nodo inicial y marcarlo como visitado
			pila_stack.push(actual);
			visitados.insert(actual);

			while(!pila_stack.empty()){
				actual = pila_stack.top();

				// si hay algun vertice no visitado, agregarlo al stack y continuar busqueda
				auto it = actual->edges.begin();
				while (it!=actual->edges.end()){
					if (visitados.find((*it)->nodes[1])==visitados.end()){
						if(isbipartite) (*it)->nodes[1]->thereis = !(*it)->nodes[0]->thereis;
						pila_stack.push((*it)->nodes[1]);
						actual = pila_stack.top();
						visitados.insert(actual);
						vector_edges_visitados.push_back(pair<N,N> ((*it)->nodes[0]->get_data(), (*it)->nodes[1]->get_data()));
						it = actual->edges.begin();
						continue;
					}
					else if(isbipartite) isbipartite = (*it)->nodes[0]->thereis != (*it)->nodes[1]->thereis;
					++it;
				}
				
				if (pila_stack.top()==actual){ 
					pila_stack.pop();
				}
			}

			if (!silenced) print_DFS(vector_edges_visitados); // comentar si no se quiere imprimir
			return vector_edges_visitados;
		};

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
		            lista.push_back(actual);
	                edge* auxedge;
		            for(auto it=actual->edges.begin(); it!=actual->edges.end(); ++it){
		                auxedge=(*it);
		                bool thereis2=false;
	                    for(auto it2=lista.begin();it2!=lista.end();++it2){
	                        if((*it2)==auxedge->nodes[1]){
	                            thereis2=true;
	                            break;
	                        }
	                    }
	                    if(!thereis2) queue_cola.push(auxedge->nodes[1]);
		            }
		        }
		    }
		    return lista;
		};

	    vector<N> BFS(N nodo_data_inicial){
			if (!(nodes.count(nodo_data_inicial)))throw "Nodo no existe";
			else {return ChangeNododirToData(BFS_nodes(nodo_data_inicial));}
		}
		vector<N> ChangeNododirToData(list<node*> lista_directions){
		    vector<N> nueva_lista;
		    for (auto& element : lista_directions){
		    	nueva_lista.push_back(element->get_data());
		    }
		    return nueva_lista;
		}

		~Graph(){
			auto it = nodes.begin();
			while (!nodes.empty()){
				delete (*it).second;
				it = nodes.erase(it);
			}
		}


};

typedef Graph<Traits> graph;

#endif
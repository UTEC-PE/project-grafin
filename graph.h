//TODO: Crear subclases de grafo para dirigido y no dirigido
//TODO: Colocar ids a los nodos

#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <utility>

#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <map>
#include <unordered_map>
#include <set>
#include <cmath>

#include "node.h"
#include "edge.h"

int const INFINITE=INT16_MAX;

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

template <typename Ma>
struct SquareMatrix {
    vector<vector<Ma>> M;

    SquareMatrix(){};
    SquareMatrix(int size, Ma thedefault){
        vector<Ma> temp(size, thedefault); 
        for (int i=0; i<size; ++i) M.push_back(temp);
    }
    
    void set(int r, int c, Ma value){ M[r][c]=value; }
    Ma get(int r, int c) { return M[r][c]; }
    int size(){ return M.size(); }
    void print(){
        cout <<endl;
        for (int i=0; i<M.size(); ++i){
            for (int j=0; j<M.size(); ++j){
                if (M[i][j]==INFINITE) cout << setw(6) <<"INF";
                else cout << setw(6) << M[i][j];
            }
            cout << endl;
        }
    }
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
        typedef typename NodeSeq::iterator NodeIte;
        typedef typename EdgeSeq::iterator EdgeIte;

    private:
        NodeSeq nodes; // Mapa de punteros de nodos <data (node name), node pointer>
        EdgeSeq edges_graph; // usado en fuertemente conexo
        
        int sizeOfGraph[2]= {0,0}; // sizeOfGraph[0]: num de nodes -  sizeOfGraph[1]: num de edges

	    Graph(NodeSeq somenodes, bool with_coord=false){ // Nuevo grafo a partir de data de vector de nodos
	    	node* newnode;
	    	for (auto pairNodes : somenodes){
                if (with_coord) add_node(pairNodes.first, pairNodes.second->x, pairNodes.second->y);
                else add_node(pairNodes.first);
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
		bool has_direction=false;

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
		bool add_node(N node_name, double x=0, double y=0){
			if (nodes.find(node_name)!=nodes.end()) return false; // name taken
			node* newnode = new node(node_name, x, y);
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

		bool remove_edge(N vi_name, N vf_name){
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
			bool* isbipartite = new bool(false);
			DFS(initial_node, true, isbipartite);

			return *isbipartite;
		};

		bool bipartite(){
			for (auto& node: nodes) {
				if (!bipartite(node.first)) 
					return false;
				MakeAllThereisFalse();
			}

			
			
			return true;
		}


		// Algoritmos en grafos 
		self kruskalAlgorithm(){
			if (has_direction) throw "Prim solo aplica a no dirigidos";

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
	    	if (has_direction) throw "Prim solo aplica a no dirigidos";
	    	MakeAllThereisFalse();

	        multimap< E ,edge*> edge_map;
	        int nodos_visitados=0;
	        Graph PrimGraph(nodes);

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


    	list<map<N,int>>Dijkstra(N dataof, bool print=false){
            if (nodes.find(dataof)==nodes.end()) throw "No existe nodo";

    		MakeAllThereisFalse();
    		list<map<N,int>> Dlist;
            int cuantos=this->DFS(dataof).size();
    		map<N, int> Dmap; // nodos , peso
    		// lleno todos con infinito y el nodo de inicio con 0
    		for(auto it=nodes.begin();it!=nodes.end();++it){
    			Dmap.insert(pair <N,int> (it->first,INFINITE));
    		}
    		Dmap[dataof]=0;
    		N wherei=dataof;
    		nodes[dataof]->thereis=true;
    		Dlist.push_back(Dmap);

    		int nodos_visitados=0;

    		// poner los valores
    		//multimap<int, N> node_map ;
    		//inserto todos los distancia del 1er nodo
    		for(auto it=nodes[dataof]->edges.begin();it!=nodes[dataof]->edges.end();++it) {
    			Dmap[(*it)->nodes[1]->get_data()] = (*it)->get_peso();
    		}
    		Dlist.push_back(Dmap);
    		nodos_visitados++;

    		//--------------------SECOND PART ------------------------------------
    		while(nodos_visitados!=cuantos){
    			wherei=BusquedaMap(Dmap);
    			//cambio el nodo donde estoy por el menor no visitado
    			nodes[wherei]->thereis=true;
    			for(auto it=nodes[wherei]->edges.begin();it!=nodes[wherei]->edges.end();++it){
    				//agrego los edges de wherei
    				if((*it)->get_peso()+Dmap[wherei] < Dmap[(*it)->nodes[1]->get_data()]){
    					//la suma del edge desde el nodo donde estoy es menor que el que esta en el mapa, lo cambio
    					Dmap[(*it)->nodes[1]->get_data()] = (*it)->get_peso()+Dmap[wherei];
    				}
    			}
    			Dlist.push_back(Dmap);
    			nodos_visitados++;
    			if(print){
    				cout<<endl<<"vec: "<<endl;
    				for(auto it=Dmap.begin();it!=Dmap.end();++it){
    					cout<<it->first<<"||"<<it->second<<endl;
    				}
    			}
    		}
    		MakeAllThereisFalse();
    		return Dlist;
    	}

    	N BusquedaMap(map <N,int> ElMapa ){
    		int menor=INFINITE ;
    		N nodo;
    		for(auto it=ElMapa.begin();it!=ElMapa.end();++it){
    			if((it->second< menor)&&(!nodes[it->first]->thereis)){
    				menor=it->second;
    				nodo= it->first;
    			}
    		}
    		return nodo;

    	}

	    void print_DFS(vector<pair<N, N>> v) {
	    	int c=0;
		    for (auto& tuple: v)
		    	cout <<"\nEdge " << ++c << ": " << tuple.first << " " << tuple.second;
		};
		
	    vector<pair<N, N>> DFS(N nodo_data_inicial, bool silenced=false, bool* isbipartite=nullptr){  // Busqueda por profundidad
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
					if (visitados.find((*it)->nodes[1])==visitados.end()){ // not found
						if(isbipartite) (*it)->nodes[1]->thereis = !(*it)->nodes[0]->thereis;
						pila_stack.push((*it)->nodes[1]);
						actual = pila_stack.top();
						visitados.insert(actual);
						vector_edges_visitados.push_back(pair<N,N> ((*it)->nodes[0]->get_data(), (*it)->nodes[1]->get_data()));
						it = actual->edges.begin();
						continue;
					}
					else if(isbipartite) *isbipartite = (*it)->nodes[0]->thereis != (*it)->nodes[1]->thereis;
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
		self GreedyBFS(N dataof, N nodofinal,bool print=false){
            if (nodes.find(dataof)==nodes.end() ||
                nodes.find(nodofinal)==nodes.end()) throw "No existe nodo";
			//if (has_direction) throw "";
			MakeAllThereisFalse();
            bool is=false;

            vector<N> result_dfs =  this->BFS(dataof);
            for (auto& el: result_dfs){
                if(el==nodofinal){
                    is=true;
                }
            }

            if(!is) throw "No se puede llegar al nodo final";

			multimap< E ,edge*> edge_map;
			int nodos_visitados=0;
			Graph GreedyBFSgraph(nodes);

			++nodos_visitados;
			nodes[dataof]->thereis=true;
			for(auto it3=nodes[dataof]->edges.begin();it3!=nodes[dataof]->edges.end();++it3){
				edge_map.insert(pair <E, edge*>((*it3)->get_peso(), *it3));
				//cout<<(*it3)->nodes[1]->get_data()<<"| "<<(*it3)->get_peso()<<endl;
			}

			while(nodes[nodofinal]->thereis==false){
				edge* aux_edge=edge_map.begin()->second;
				//cout<<"maybe next edge pair : "<<aux_edge->nodes[0]->get_data()<<","<<aux_edge->nodes[1]->get_data()<<"||"<<endl;
				edge_map.erase(edge_map.begin());
				if(!aux_edge->nodes[1]->thereis){
					//cout<<"si"<<endl;
					aux_edge->nodes[1]->thereis=true;
					if(print) {
						cout<<aux_edge->nodes[0]->get_data()<<","<<aux_edge->nodes[1]->get_data()<<"||"<<endl;
					}

					GreedyBFSgraph.add_edge((aux_edge)->nodes[0]->get_data(),(aux_edge)->nodes[1]->get_data(),(aux_edge)->get_peso(),0);
					for(auto it3=aux_edge->nodes[1]->edges.begin();it3!=aux_edge->nodes[1]->edges.end();++it3){
						//cout<<"edges++"<<endl;
						edge_map.insert(pair <E, edge*>( (*it3)->get_peso(), *it3));
					}
					++nodos_visitados;
				}
			}
			MakeAllThereisFalse();

			return GreedyBFSgraph;
		}

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

        pair<SquareMatrix<int>,SquareMatrix<int>> floydWarshall(){
            unordered_map<N, int> nodename_to_id;

             // Fill nodename-id maps
            int c=0;
            for (auto& thenode: nodes){
                nodename_to_id[thenode.first] = c++;
            }

            // Fill distances
            SquareMatrix<int> distances(sizeOfGraph[0], INFINITE);

            for (auto& thenode: nodes)
                for (auto& theedge: thenode.second->edges)
                    distances.set(nodename_to_id[thenode.first],
                                           nodename_to_id[theedge->nodes[1]->get_data()],
                                           theedge->get_peso());
           
            // Initialize steps
            SquareMatrix<int> steps;
            c=0;
            vector<int> temp;
            for (int i=0; i<sizeOfGraph[0]; ++i) temp.push_back(c++);
            for (int i=0; i<sizeOfGraph[0]; ++i) steps.M.push_back(temp);


            // Fill diagonal of both matrices with zeros
            for(int i=0; i<sizeOfGraph[0]; ++i){
                steps.set(i, i, 0);
                distances.set(i, i, 0);
            }

 
            for (int k=0; k<steps.size(); ++k){
                for (int i=0; i<steps.size(); ++i){
                    for (int j=0; j<steps.size(); ++j){
                        if(distances.get(i, j) > distances.get(i,k)+distances.get(k,j)){
                            distances.set(i, j, distances.get(i,k)+distances.get(k,j));
                            steps.set(i, j, k);
                        }
                    }
                }
            }

            return make_pair(steps, distances);
        }

        map<N, int> bellmanFord(N initial_node, bool print=false){
            if (nodes.find(initial_node)==nodes.end()) throw "No existe nodo";
            if (!has_direction) throw "Bellman Ford solo aplica a dirigidos";
            map<N, int> distances;
             // Initialize distances with infinite
            for (auto& thenode: nodes) distances[thenode.first] = INFINITE;
            distances[initial_node] = 0;


            bool change=true;
            for (int i=0; i<distances.size(); ++i){
                for (auto& thenode: nodes){
                    for (auto& theedge: thenode.second->edges){
                        N nbegin = theedge->nodes[0]->get_data();
                        N nend = theedge->nodes[1]->get_data();
                        if (distances[nbegin] + theedge->get_peso() < distances[nend]){
                            change = true;
                            distances[nend] = distances[nbegin] + theedge->get_peso();
                        }
                    }
                }
                change = false;
            }

            if (print)
                for (auto& el: distances) cout << "\nDistance to " << el.first << " is " << el.second;
            
            return distances;
        }

        self A_asterisco(N initial_node, N final_node, bool print=false){
            self ans(nodes); // grafo que almacenara el camino

            // Setear heuristicas de todos los nodos
            map<N, double> heuristica;
            for (auto& thenode: nodes) 
                heuristica[thenode.first] = 
                    sqrt(pow(thenode.second->x - nodes[final_node]->x, 2) +
                        pow(thenode.second->y - nodes[final_node]->y, 2));

            // por_visitar = (node_name, heuristica+distancia)
            // la distancia del primer nodo es 0, solo se añade su heuristica
            vector<pair<N, double>> por_visitar = {pair<N, int>(initial_node, heuristica[initial_node])};
            map<N, int> distancia = {pair<N, int>(initial_node, 0)};

            // mientras que encuentre final_node y no este vacio
            while (!por_visitar.empty() && distancia.find(final_node)==distancia.end()){
                sort(por_visitar.begin(), por_visitar.end(), [](auto &left, auto &right){
                                        return left.second < right.second;
                                    });
                 
                // Seleccionar nodo con menor heuristica+distancia
                N selected_node = por_visitar[0].first;
                por_visitar.erase(por_visitar.begin());

                // Recorrer los edges del nodo seleccionado
                for (auto& theedge: nodes[selected_node]->edges){
                    N nodo_anadir = theedge->nodes[1]->get_data();
                    if (distancia.insert(make_pair(nodo_anadir, distancia[selected_node]+theedge->get_peso() )).second){
                        // si el elemento no estaba en el mapa, añadir a por_visitar
                        // y al grafo answer
                        por_visitar.push_back( make_pair(nodo_anadir, heuristica[nodo_anadir]+distancia[nodo_anadir]));
                        if (print) {
                            cout << "\nNodo " << selected_node << " -> Nodo " <<nodo_anadir;
                            if (nodo_anadir==final_node) cout << " *** Camino correcto";
                        }    
                    }
                    else if ( distancia[nodo_anadir] > distancia[selected_node]+theedge->get_peso() ){
                        distancia[nodo_anadir] = distancia[selected_node]+theedge->get_peso();
                    }
                    ans.add_edge(*theedge);
                }
            }

            // Si no se encontro un camino, devolver el mismo grafo sin edges
            if (por_visitar.empty()) {
                if (print) cout <<"\nNo existe un camino";
            }
            
            return ans;
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
#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <iostream>
#include <boost/unordered_map.hpp>
//#include <unordered_map>
#include <utility>
#include <vector>

//#include "utility.hpp"


template<class T>
class graph_t {
    private:
        typedef boost::unordered_map<int, std::vector<int>* > adj_hmap_t;
        typedef boost::unordered_map<int, T> ndata_hmap_t;
        typedef boost::unordered_map<std::pair<int, int>, T> edata_hmap_t;
        int num_nodes;
    public:
        adj_hmap_t edges;
        ndata_hmap_t node_data;
        edata_hmap_t edge_data;

        graph_t() : num_nodes(0) {}
        ~graph_t(){
        }

        int get_num_nodes(){
            return num_nodes;
        }

        int add_vertex(){
            return num_nodes++;
        }
        
        std::pair<int,int> add_edge(int from, int to){
            if(edges.find(from) != edges.end()){ // if from exists in edges
                std::vector<int> *vect = edges[from];
                (*vect).push_back(to);
            }
            else {
                std::vector<int> *vect = new std::vector<int>();
                (*vect).push_back(to);
                edges.insert(adj_hmap_t::value_type(from, vect));
            }

            return std::make_pair(from, to);
        }

        void print_edges(){
            //print all edges of all vertices
            for(auto it = (*this).edges.begin(); it != (*this).edges.end(); ++it){
                std::cout << "Edges of node " << (*it).first << " :" << std::endl;

                for(auto it2 = it->second->begin(); it2 != it->second->end(); ++it2){
                    std::cout << "\t" << (*it).first << " -> " << *it2 << std::endl;
                }

                std::cout << std::endl;
            }
        }

        void print_edges(int n){
            //print edges of node n
            if(edges.find(n) != edges.end()){
                std::cout << "Edges of node " << n << " :" << std::endl;

                for(auto it2 = edges[n]->second->begin(); it2 != edges[n]->second->end(); ++it2){
                    std::cout << "\t" << n << " -> " << *it2 << std::endl;
                }

                std::cout << std::endl;
            }
            else std::cout << "Node " << n << " has no edges." << std::endl;
        }
};

#endif

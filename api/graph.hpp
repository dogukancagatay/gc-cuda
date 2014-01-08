#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

#include "utility.h"

template<class T>
class graph_t {
    private:
        int num_nodes;
    public:
        std::unordered_map<int, std::vector<int>* > edges;
        std::unordered_map<int, T> node_data;
        std::unordered_map<std::pair<int, int>, T> edge_data;

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
                edges.insert(std::make_pair(from, vect));
            }

            return std::make_pair(from, to);
        }
};

#endif

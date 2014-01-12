#include <iostream>
#include <boost/unordered_map.hpp>
#include <utility>
#include <vector>
#include <string>
#include <cstdlib>

#include "graph.hpp"

//typedef boost::unordered_map<int, std::vector<int>* > adj_hmap_t;
//typedef boost::unordered_map<int, T> ndata_hmap_t;
//typedef boost::unordered_map<std::pair<int, int>, T> edata_hmap_t;
//typedef boost::unordered_map<int, int> count_hmap_t;

void graph_t::set_num_nodes(int num){
    this->num_nodes = num;
}

int graph_t::get_num_nodes(){
    return this->num_nodes;
}

int graph_t::add_vertex(){
    return this->num_nodes++;
}

std::pair<int,int> graph_t::add_edge(int from, int to){
    if(this->edges.find(from) != this->edges.end()){ // if from exists in edges
        std::vector<int> *vect = this->edges[from];
        (*vect).push_back(to);
    }
    else {
        std::vector<int> *vect = new std::vector<int>();
        (*vect).push_back(to);
        this->edges.insert(adj_hmap_t::value_type(from, vect));
    }

    return std::make_pair(from, to);
}

void graph_t::print_edges(){
    //print all edges of all vertices
    for(auto it = this->edges.begin(); it != this->edges.end(); ++it){
        std::cout << "Edges of node " << (*it).first << " :" << std::endl;

        for(auto it2 = it->second->begin(); it2 != it->second->end(); ++it2){
            std::cout << "\t" << (*it).first << " -> " << *it2 << std::endl;
        }

        std::cout << std::endl;
    }
}

void graph_t::print_edges(int n){
    //print edges of node n
    if(this->edges.find(n) != this->edges.end()){
        std::cout << "Edges of node " << n << " :" << std::endl;

        for(auto it2 = this->edges[n]->begin(); it2 != this->edges[n]->end(); ++it2){
            std::cout << "\t" << n << " -> " << *it2 << std::endl;
        }

        std::cout << std::endl;
    }
    else std::cout << "Node " << n << " has no edges." << std::endl;
}

graph_t::~graph_t(){
    for(auto it=edges.begin(); it != edges.end(); ++it){
        delete it->second;
    }

    free(shard_to_node);
}

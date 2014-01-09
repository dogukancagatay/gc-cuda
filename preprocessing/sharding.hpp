#ifndef SHARDING_HPP
#define SHARDING_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <boost/unordered_map.hpp>
//#include <unordered_map>
#include <utility>
#include <math.h>

#define MB_TO_BYTES 1024*1024

#include "../api/utility.hpp"

typedef boost::unordered_map<int, int> hash_t;

void shard_graph(params* par, char* gfilename){
    /* Open the graph file */
    std::ifstream gfile(gfilename);
    hash_t in_edge_count; // node <-- number of in edges

    int num_edges = 0;

    if(gfile.is_open()){
        std::string eline;
        while(getline(gfile, eline)){
            /* get an edge from the file */
            std::istringstream buff(eline);
            int from, to;

            buff >> from;
            buff >> to;

            if(in_edge_count.find(to) == in_edge_count.end()) { //if the node is encountered for the first time
                in_edge_count.insert(hash_t::value_type(to,1));
            }
            else {
                in_edge_count[to] += 1;
            }
        }

        for(auto it = in_edge_count.begin(); it != in_edge_count.end(); ++it){
            std::cout << it->first << " : " << it->second << std::endl;
            num_edges += it->second;
        }

        std::cout << "Number of nodes = " << in_edge_count.size() << std::endl;
        std::cout << "Number of edges = " << num_edges << std::endl;

        gfile.close();
    }
    else {
        std::cout << "Unable to open file" << std::endl;
        exit(1); //exit with error
    }

    /* calculate number of shards needed */
    int membudget_b = par->mem_budget * MB_TO_BYTES;
    par->max_num_edges = (int)floor((double)membudget_b / (double)par->edge_mem_cost);
    par->num_shards = (int)ceil((double)num_edges / (double)par->max_num_edges);


    std::cout << "Max number of edges per shard = " << par->max_num_edges << std::endl;
    std::cout << "Number of shards = " << par->num_shards << std::endl;

    /* create files for shards */ 
    /* assign vertices to shards */
    /* scan the graph file and start writing to shards */

}

#endif

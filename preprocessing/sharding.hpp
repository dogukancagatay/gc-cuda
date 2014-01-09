#ifndef SHARDING_HPP
#define SHARDING_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <utility>

//#include "api/graph.h"

void foo(char* gfilename){
    /* Open the graph file */
    std::ifstream gfile(gfilename);
    std::string eline;
    std::unordered_map<int,int> in_edge_count; // node <-- number of in edges

    if(gfile.is_open()){
        while(getline(gfile, eline)){
            /* get an edge from the file */
            std::istringstream buff(eline);
            int from, to;

            buff >> from;
            buff >> to;

            if(in_edge_count.find(to) == in_edge_count.end()) { //if the node is encountered for the first time
                in_edge_count.insert(std::make_pair(to,1));
            }
            else {
                in_edge_count[to] += 1;
            }
        }

        int num_edges = 0;
        for(auto it = in_edge_count.begin(); it != in_edge_count.end(); ++it){
            std::cout << it->first << " : " << it->second << std::endl;
            num_edges += it->second;
        }

        std::cout << "Number of nodes = " << in_edge_count.size() << std::endl;
        std::cout << "Number of edges = " << num_edges << std::endl;

        gfile.close();
    }
    else std::cout << "Unable to open file" << std::endl;
}

#endif

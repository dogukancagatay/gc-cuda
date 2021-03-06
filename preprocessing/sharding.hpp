#ifndef SHARDING_HPP
#define SHARDING_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <boost/unordered_map.hpp>
#include <utility>
#include <math.h>
#include <memory>
#include <map>
#include <cstdlib>

#define MB_TO_BYTES 1024*1024

#include "../api/utility.hpp"

typedef boost::unordered_map<int, int> hash_t;

void shard_graph(params* par, graph_t* g, char* gfilename){
    /* Open the graph file */
    std::ifstream gfile(gfilename);
    //hash_t g->in_edge_counts; // node <-- number of in edges


    if(gfile.is_open()){
        std::string eline;
        while(getline(gfile, eline)){
            /* get an edge from the file */
            std::istringstream buff(eline);
            int from, to;

            buff >> from;
            buff >> to;

            if(g->in_edge_counts.find(to) == g->in_edge_counts.end()) { //if the node is encountered for the first time
                g->in_edge_counts.insert(hash_t::value_type(to,1));
            }
            else {
                g->in_edge_counts[to] += 1;
            }

            if(g->out_edge_counts.find(from) == g->out_edge_counts.end()) { //if the node is encountered for the first time
                g->out_edge_counts.insert(hash_t::value_type(from,1));
            }
            else {
                g->out_edge_counts[from] += 1;
            }
        }

        g->num_nodes = g->in_edge_counts.size();

#ifdef DEBUG
        std::cout << "In edge counts:" << std::endl;
#endif
        for(auto it = g->in_edge_counts.begin(); it != g->in_edge_counts.end(); ++it){
#ifdef DEBUG
            std::cout << "\t" << it->first << " : " << it->second << std::endl;
#endif
            g->num_edges += it->second;
        }

#ifdef DEBUG
        std::cout << "Out edge counts:" << std::endl;
#endif
        for(auto it = g->out_edge_counts.begin(); it != g->out_edge_counts.end(); ++it){
#ifdef DEBUG
            std::cout << "\t" << it->first << " : " << it->second << std::endl;
#endif
        }

        std::cout << "Number of nodes = " << g->num_nodes << std::endl;
        std::cout << "Number of edges = " << g->num_edges << std::endl;

        //gfile.close();
    }
    else {
        std::cout << "Unable to open file (#1). Exiting..." << std::endl;
        exit(1); //exit with error
    }

    /* calculate number of shards needed */
    float membudget_b = par->mem_budget * MB_TO_BYTES;
    par->max_num_edges = ((int)floor((double)membudget_b / (double)par->edge_mem_cost)) / 2; // over 2 b/c assuming vertex_out = vertex_in
    par->num_shards = (int)ceil((double)g->num_edges / (double)par->max_num_edges);
    g->num_shards = par->num_shards;


    std::cout << "Max number of edges per shard = " << par->max_num_edges << std::endl;
    std::cout << "Number of shards = " << par->num_shards << std::endl;

    /* assign vertices to shards */
    hash_t node_to_shard; // dest_node <-- shard_id
    
    for(int i = 0; i < par->num_shards; ++i){
    }

    int shard_index = 0;

    int shard_edge_count = 0;
    //for(auto it = g->in_edge_counts.begin(); it != g->in_edge_counts.end(); ++it){
    for(int i = 0; i < g->in_edge_counts.size(); ++i){
        // when we add the that node's edges to that shard, does it cross the max number of edges limit per shard
        if(shard_edge_count + g->in_edge_counts[i] > par->max_num_edges){
            shard_index++; // move to the next shard
            shard_edge_count = 0;
        }

        node_to_shard.insert(hash_t::value_type(i, shard_index)); // assign node -> shard[i] 
        shard_edge_count+= g->in_edge_counts[i]; //add the edge count to shard edge count

    }

    /* cleanning some memory */
    //g->in_edge_counts.clear();

    //assign shards to vertices
    //g->shard_to_node holds shard to node assignments

    //g->shard_to_node.resize(par->num_shardsi + 1); //allocate memory
    g->shard_to_node = (int*) malloc((par->num_shards + 1) * sizeof(int));//allocate memory
    for(int i = 0; i < par->num_shards; ++i){
        int min_dest = 2000000000;

#ifdef DEBUG
        std::cout << "Nodes in shard " << i << ":  " << std::endl;
#endif
        for(auto it=node_to_shard.begin(); it != node_to_shard.end(); ++it){
            if(it->second == i){
#ifdef DEBUG
                std::cout << "\t" << it->first << std::endl;
#endif

                if(it->first < min_dest)
                    min_dest = it->first;
            }
        }
        g->shard_to_node[i] = min_dest;
        
    }
    //last element is number of nodes
    g->shard_to_node[par->num_shards] = g->num_nodes;


    /* create files for shards */ 
    std::vector<std::shared_ptr<std::ofstream> > ofs_shards;

    for(int i = 0; i< par->num_shards; ++i){
        std::ostringstream buff;
        buff << i << "_shard_";
        buff << get_fname_from_path(gfilename);

        g->shard_fnames.push_back(buff.str());

        std::cout << "Shard file created : " << buff.str() << std::endl;

        ofs_shards.push_back(std::make_shared<std::ofstream>(buff.str()));
    }

    /* scan the graph file and start writing to shards */
    gfile.clear(); // clear the state of file stream 
    gfile.seekg(0, std::ios::beg); // go to the beginning of the file
    
    if(gfile.is_open()){
        std::string eline;
        while(getline(gfile, eline)){
            /* get an edge from the file */
            std::istringstream buff(eline);
            int from, to;

            buff >> from;
            buff >> to;

#ifdef DEBUG
            std::cout<< "Writing to shard " << node_to_shard[to] << " dest " << to << std::endl;
#endif
            //add the edge to its assigned shard
            (*(ofs_shards[node_to_shard[to]])) << from << " " << to << std::endl;
        }

        gfile.close();
    }
    else {
        std::cout << "Unable to open file (#2). Exiting..." << std::endl;
        exit(1);
    }

    /* close shard files that are opened for writing*/
    for(int i = 0; i< par->num_shards; ++i){
        ofs_shards[i]->close();
    }
    
    /* cleanning some memory */
    ofs_shards.clear();

    /* sorting shard files according to their source files */
    std::cout << "Starting sorting shard files." << std::endl;

    /* read the shard files */
    std::vector<std::shared_ptr<std::ifstream> > ifs_shards;

    for(int i = 0; i< par->num_shards; ++i){
        std::cout<< "Reading shard file : " << g->shard_fnames[i] << std::endl;

        //std::ifstream tempf(g->shard_fnames[i]);
        //ifs_shards.push_back(&tempf);
        ifs_shards.push_back(std::make_shared<std::ifstream>(g->shard_fnames[i]));
    }
    
    std::cout << "Writing sorted shard files."<< std::endl;
    /* sort each shard according to their source node and write them to their file  */
    for(int i = 0; i< par->num_shards; ++i){
        std::map<int, std::vector<int> > shard_edge_list; // from --> to1,to2,to3

        //read first
        if(ifs_shards[i]->is_open()){
            std::string eline;
            while(getline(*ifs_shards[i], eline)){
                /* get an edge from the file */
                std::istringstream buff(eline);
                int from, to;

                buff >> from;
                buff >> to;

                if(shard_edge_list.find(from) == shard_edge_list.end()){ // if not in the map
                    std::vector<int> temp_v;
                    temp_v.push_back(to);

                    shard_edge_list.insert(std::make_pair(from,temp_v));
                }
                else {
                    shard_edge_list[from].push_back(to);
                }
            }

            ifs_shards[i]->close();
        }
        else {
            std::cout << "Unable to open file (#3). Exiting..." << std::endl;
            exit(1);
        }

        //write the sorted edges
        std::ofstream ofs_shard(g->shard_fnames[i]);
        
        //for each source node
        for(auto it = shard_edge_list.begin(); it != shard_edge_list.end(); ++it){
            //write the adj list to the shard in sorted order
            for(auto it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2){
                ofs_shard << (*it).first << " " << (*it2) << " " << (double)(0.0) << std::endl; 
            }
        }

        ofs_shard.close();
    }

    std::cout << "Initial sharding is finished." << std::endl;

}
#endif

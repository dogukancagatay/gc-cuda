#include <iostream>

#include "api/graph.hpp"
#include "api/utility.hpp"
#include "preprocessing/sharding.hpp"
#include "timing.h"

/*#define DEBUG*/

using namespace std;

void gc_cuda(params *par, graph_t *graph);

int main(int argc, char** argv){
    /* keep global program parameters in par */
    params par;
    par.mem_budget = 600;
    /*par.mem_budget = 0.000740;*/
    par.edge_mem_cost = 30; // a fairly good cost
    par.num_iterations = 10; // for pagerank

    graph_t g;

    //shard_graph(&par, &g, "data/kucuk/kucuk.txt");
    //shard_graph(&par, &g, "data/wiki-Vote/wiki-Vote.txt");
    shard_graph(&par, &g, "data/twitter_combined/twitter_combined.txt");
    //shard_graph(&par, &g, "data/sekizli/sekizli.txt");

    for(int i = 0; i< par.num_shards + 1; i++){
        cout << g.shard_to_node[i] << endl;
    }
    gc_cuda(&par, &g);
    
    return 0;
}

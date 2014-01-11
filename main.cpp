#include <iostream>

#include "api/graph.hpp"
#include "api/utility.hpp"
#include "preprocessing/sharding.hpp"

using namespace std;

void gc_cuda(params *par, graph_t *graph);

int main(int argc, char** argv){
    /* keep global program parameters in par */
    params par;
    par.mem_budget = 0.000370;
    par.edge_mem_cost = 60; // a fairly good cost
    graph_t g;

    shard_graph(&par, &g, "data/kucuk.txt");
    
    gc_cuda(&par, &g);

    return 0;
}

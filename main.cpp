#include <iostream>

#include "api/graph.hpp"
#include "api/utility.hpp"
#include "preprocessing/sharding.hpp"


using namespace std;

int main(int argc, char** argv){
    /* keep global program parameters in par */
    params par;
    par.mem_budget = 800;
    par.edge_mem_cost = 60; // a fairly good cost
    //par.edge_mem_cost = 24; // (an edge : 2 * 4 bytes = 8 bytes) + (hashmap cost = 16 bytes) = 24 bytes
    //par.edge_mem_cost = 32; // keeping a double for each edge +8 bytes
    //par.edge_mem_cost = 48; // keeping a double for each node so +(2 * 8 bytes) 

    // Sample graph
    graph_t<int> g;

    //add vertices
    g.add_vertex();
    g.add_vertex();
    g.add_vertex();
    g.add_vertex();
    g.add_vertex();

    //add edges
    g.add_edge(0,1);
    g.add_edge(2,1);
    g.add_edge(3,0);
    g.add_edge(4,0);
    g.add_edge(4,1);
    g.add_edge(5,1);

    g.add_edge(0,2);
    g.add_edge(1,2);
    g.add_edge(2,3);
    g.add_edge(4,2);
    g.add_edge(5,3);

    g.add_edge(1,4);
    g.add_edge(2,4);
    g.add_edge(2,5);
    g.add_edge(3,4);
    g.add_edge(4,5);

    g.print_edges();

    shard_graph(&par, "data/kucuk.txt");

    return 0;
}

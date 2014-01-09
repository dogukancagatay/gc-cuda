#include <iostream>
#include "api/graph.hpp"

typedef struct global_parameters {
    int nshards;
    int membudget;
} params;

using namespace std;

int main(int argc, char** argv){
    /* keep global program parameters in par */
    params par;

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

    //print all edges of all vertices
    for(int i = 0; i < g.get_num_nodes(); i++){
        cout << "Edges of node " << i << " :" <<endl;
        for(auto it = g.edges[i]->begin(); it != g.edges[i]->end(); ++it){
            cout << "\t" << i << " -> " << *it << endl;
        }
        cout << endl;
    }

    return 0;
}

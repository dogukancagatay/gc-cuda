#include <iostream>
#include "api/graph.hpp"
#include "preprocessing/sharding.hpp"

typedef struct global_parameters {
    int nshards;
    int membudget;
} params;

using namespace std;

//template<class T>
//void print_edges(graph_t<T>& g){
    ////print all edges of all vertices
    //for(auto it = g.edges.begin(); it != g.edges.end(); ++it){
        //cout << "Edges of node " << (*it).first << " :" <<endl;

        //for(auto it2 = it->second->begin(); it2 != it->second->end(); ++it2){
            //cout << "\t" << (*it).first << " -> " << *it2 << endl;
        //}

        //cout << endl;
    //}
//}

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

    g.print_edges();

    foo("kucuk.txt");

    return 0;
}

#include <iostream>
#include <boost/program_options.hpp>
#include "api/graph.hpp"

typedef struct global_parameters {
    int nshards;
    int membudget;
} params;
namespace po = boost::program_options;
using namespace std;

int main(int argc, char** argv){
    /* keep global program parameters in par */
    params par;

    /* --- getting arguments with boost program_options --- */
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "Produces help message")
        ("nshards,s", po::value<int>(&(par.nshards))->default_value(-1),
                  "Number of memory shards the graph is going to be partitioned.")
        ("membudget,m", po::value<int>(&(par.membudget))->default_value(800),
                  "Memory amount in MB that can be used by the program. Set to 70% of your total memory.")
        ;

    po::positional_options_description p;
    p.add("help", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << "Usage: options_description [options]" << std::endl;
        std::cout << desc;
        return 0;
    }
    /* -------------------------------------------------- */

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

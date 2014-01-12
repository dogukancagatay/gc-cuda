#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <utility>

int main(int argc, char** argv){
    if(argc == 3){
        /* Open the graph file */
        std::ifstream gfile(argv[1]);
        std::ofstream ofile(argv[2]);

        std::string eline;
        //std::unordered_map<int,int> in_edge_count; // node <-- number of in edges
        std::unordered_map<std::string,int> labels; // node_label <-- node_id
        
        int node_count = 0;

        if(gfile.is_open()){
            while(getline(gfile, eline)){
                /* get an edge from the file */
                std::istringstream buff(eline);
                std::string from_label, to_label;
                int from, to;

                buff >> from_label;
                buff >> to_label;

                if(labels.find(from_label) == labels.end()) { //if the node is encountered for the first time
                    from = node_count++;
                    labels.insert(std::make_pair(from_label,from)); //index labelled node to a node id
                    //in_edge_count.insert(std::make_pair(from,0));
                }
                else {
                    from = labels[from_label];
                }

                if(labels.find(to_label) == labels.end()) { //if the node is encountered for the first time
                    to = node_count++;
                    labels.insert(std::make_pair(to_label, to)); //index labelled node to a node id
                    //in_edge_count.insert(std::make_pair(to,0));
                }
                else {
                    to = labels[to_label];
                    //in_edge_count[labels[from_label]] += 1;
                }
                
                //add to converted labels file
                ofile << from << " " << to << std::endl;
            }

            gfile.close();
            ofile.close();
        }
        else std::cout << "Unable to open input file for reading." << std::endl;
    }
    else std::cout << "Converts the original graph file format to gc-cuda graph file format\nUsage: original_graph_file(input) converted_graph_file(output)\n" << std::endl;
}

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <utility>

int main(int argc, char** argv){
    if(argc == 5){
        /* Open the graph file */
        std::ifstream gfile(argv[1]);
        std::ifstream afile(argv[2]);
        std::ofstream ofile(argv[3]);
        int num_cols = atoi(argv[4]);

        std::string eline;
        std::unordered_map<int,int> labels; // node_label <-- node_id taken from original graph file
        std::unordered_map<int,int> rev_labels; // node_id <-- node_label
        
        int node_count = 0;

        if(gfile.is_open()){
            while(getline(gfile, eline)){

                /* get an edge from the file */
                std::istringstream buff(eline);
                int from_label, to_label, from, to;

                buff >> from_label;
                buff >> to_label;

                if(labels.find(from_label) == labels.end()) { //if the node is encountered for the first time
                    from = node_count++;
                    labels.insert(std::make_pair(from_label,from)); //index labelled node to a node id
                    rev_labels.insert(std::make_pair(from,from_label)); //index labelled node to a node id
                }

                if(labels.find(to_label) == labels.end()) { //if the node is encountered for the first time
                    to = node_count++;
                    labels.insert(std::make_pair(to_label, to)); //index labelled node to a node id
                    rev_labels.insert(std::make_pair(to,to_label)); //index labelled node to a node id
                }

            }
            gfile.close();

            std::cout << "printing labels : " << std::endl;
            for(auto it=labels.begin(); it != labels.end(); ++it){
                std::cout << it->first << " ->  " << it->second << std::endl;
            }
            std::cout << "printing rev labels : " << std::endl;
            for(auto it=rev_labels.begin(); it != rev_labels.end(); ++it){
                std::cout << it->first << " ->  " << it->second << std::endl;
            }



            if(afile.is_open()){
                while(getline(afile, eline)){

                    /* get an edge from the file */
                    std::istringstream buff(eline);
                    int node_id, orig_label;

                    for(int i = 0; i < num_cols; i++){
                        if(i != 0){
                            ofile << " "; // put space character between values
                        }
                        buff >> node_id;

                        if(rev_labels.find(node_id) != rev_labels.end()) { //if node id in gc format exists in our table
                            orig_label = rev_labels[node_id];
                            ofile << orig_label;
                        }
                        else std::cout<< node_id <<" not in the map" << std::endl;
                    }

                    ofile << std::endl; // after reading all values on the line, end the line in output file.
                }
                afile.close();
                ofile.close();
            }
            else std::cout << "Unable to open second input file for reading." << std::endl;
        }
        else std::cout << "Unable to open first input file for reading." << std::endl;
    }
    else std::cout << "Converts any graph file in the gc-cuda graph file format to original graph file format\nUsage: original_graph_file(input) a_graph_file(input) converted_graph_file(output) number_of_columns_to_be_changed(positive integer)\n" << std::endl;

    return 0;
}

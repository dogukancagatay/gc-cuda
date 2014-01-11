#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <boost/unordered_map.hpp>
#include <utility>
#include <vector>
#include <string>


class graph_t {
    private:
        typedef boost::unordered_map<int, std::vector<int>* > adj_hmap_t;
        typedef boost::unordered_map<int, double> ndata_hmap_t;
        typedef boost::unordered_map<std::pair<int, int>, double> edata_hmap_t;
        typedef boost::unordered_map<int, int> count_hmap_t;
    public:
        adj_hmap_t edges;
        ndata_hmap_t node_data;
        edata_hmap_t edge_data;
        count_hmap_t in_edge_counts;
        count_hmap_t out_edge_counts;
        std::vector<std::string> shard_fnames;

        int num_nodes;
        int num_edges;

        graph_t() : num_nodes(0), num_edges(0) {}
        ~graph_t(){}
        void set_num_nodes(int num);
        int get_num_nodes();
        int add_vertex();
        std::pair<int,int> add_edge(int from, int to);
        void print_edges();
        void print_edges(int n);
};

#endif

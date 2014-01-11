#include <iostream>
#include <vector>
#include <string>

#include "cuda.h"
#include "cuda_runtime.h"

#include "api/utility.hpp"
#include "api/graph.hpp"

using namespace std;

void gc_cuda(params *par, graph_t *graph){
    
    // number of shards, filenames of shards
    for(int i=0;i<par->num_shards; i++) {
        std::cout<<graph->shard_fnames[i]<< "\n";
    }
}


void deneme(){
}

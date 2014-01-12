#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>

#include <boost/unordered_map.hpp>

#include "cuda.h"
#include "cuda_runtime.h"

#include "api/utility.hpp"
#include "api/graph.hpp"

#define threadsPerBlock 32
#define RANDOMRESETPROB 0.15

using namespace std;

__global__ void vertexUpdate(int iteration, int activeShardLength, float *activeShardContent_d, int firstActiveVertex, int firstActiveVertexInNextShard, float *result_d) {
	int idx = blockIdx.x * blockDim.x + threadIdx.x;
	int assignedVertexId = idx + firstActiveVertex;
	
	float src, dest, value;
	float pagerank = 1, sum;

	if(assignedVertexId<firstActiveVertexInNextShard) {
		if(iteration != 0) {
			sum = 0;
			
			for(int i=0; i<activeShardLength; i++) {
				src = activeShardContent_d[i*3+0];
				dest = activeShardContent_d[i*3+1];
				value = activeShardContent_d[i*3+2];

				if(dest == assignedVertexId) {
					sum = sum + value;
				}
			}

			pagerank = RANDOMRESETPROB + (1 - RANDOMRESETPROB) * sum;
		} 

		result_d[idx] = pagerank/result_d[idx];
	}
}

int getActiveShardLength(int firstActiveVertex, int firstActiveVertexInNextShard, graph_t *graph) {
	int total = 0;

	for(int i=firstActiveVertex; i<firstActiveVertexInNextShard; i++)
		total = total + graph->in_edge_counts[i];

	return total;
}

void gc_cuda(params *par, graph_t *graph){

	cout<<endl<<endl<<"gc_cuda"<<endl;
	int num_iterations = par->num_iterations;

    FILE **ifShards = (FILE **)malloc(sizeof(FILE *) * par->num_shards);
    FILE **tfShards = (FILE **)malloc(sizeof(FILE *) * par->num_shards);
    int *srcs = (int *)malloc(sizeof(int) * par->num_shards);
    
    for(int i=0; i<num_iterations; i++) {
    	cout << "------------iteration: "<<i<<"------------"<<endl<<endl;
    	// open shards to read
    	for(int j=0; j<par->num_shards; j++) {
    		ifShards[j] = fopen(graph->shard_fnames[j].c_str(), "r");
    	}

    	// open temp shards to write
    	for(int j=0; j<par->num_shards; j++) {
    		tfShards[j] = fopen(("temp"+graph->shard_fnames[j]).c_str(), "w");
    	}

    	// init srcs with -1
    	for(int j=0; j<par->num_shards; j++) {
    		srcs[j] = -1;
    	}

    	for(int activeShard=0; activeShard<par->num_shards; activeShard++){
    		FILE *ifActiveShard = fopen(graph->shard_fnames[activeShard].c_str(), "r");

    		int firstActiveVertex = graph->shard_to_node[activeShard];
    		int firstActiveVertexInNextShard = graph->shard_to_node[activeShard+1];
			int activeShardLength = getActiveShardLength(firstActiveVertex, firstActiveVertexInNextShard, graph);

			// memory alloc on host and device for active shard content
			float *activeShardContent;
			float *activeShardContent_d;

			activeShardContent = (float *)malloc(activeShardLength * 3 * sizeof(float));
			cudaMalloc((void **) &activeShardContent_d, activeShardLength * 3 * sizeof(float));
						
			// memory alloc on host and device for kernel result
			// result initially stores out edge counts
			float *result;
			float *result_d;

			result = (float *)malloc((firstActiveVertexInNextShard - firstActiveVertex) * sizeof(float));
			for(int k=0; k<(firstActiveVertexInNextShard - firstActiveVertex); k++)
				result[k] = graph->out_edge_counts[k+firstActiveVertex];
			cudaMalloc((void **) &result_d, (firstActiveVertexInNextShard - firstActiveVertex) * sizeof(float));

			// read active shard content
			for(int k=0; k<activeShardLength*3; k++){
				fscanf(ifActiveShard,"%f ", &activeShardContent[k]);
			}

			// copy active shard from host to device
			cudaMemcpy(activeShardContent_d, activeShardContent, activeShardLength * 3 * sizeof(float), cudaMemcpyHostToDevice);
			cudaMemcpy(result_d, result, (firstActiveVertexInNextShard - firstActiveVertex) * sizeof(float), cudaMemcpyHostToDevice);

			// vertex update
			int blocksPerGrid = (firstActiveVertexInNextShard - firstActiveVertex + threadsPerBlock -1)/threadsPerBlock;
			vertexUpdate<<<blocksPerGrid,threadsPerBlock>>>(i, activeShardLength, activeShardContent_d, firstActiveVertex, firstActiveVertexInNextShard, result_d);

			// copy output from device to host
			cudaMemcpy(result, result_d, (firstActiveVertexInNextShard - firstActiveVertex) * sizeof(float), cudaMemcpyDeviceToHost);

			// write output to temp shard
			int src, dest;
			float value;
			cout<<"activeShard: "<<activeShard<<endl;
			for(int k=0; k<par->num_shards; k++) {
				while(true) {
					if(feof(ifShards[k]))
						break;

					if(srcs[k] == -1) {
						fscanf(ifShards[k], "%d ",&src);
					}
					else {
						src = srcs[k];
						srcs[k] = -1;
					}

					if(src < firstActiveVertexInNextShard) {
						fscanf(ifShards[k], "%d %f ",&dest, &value);
						fprintf(tfShards[k], "%d %d %f\n", src, dest, result[src - firstActiveVertex]);
						//cout<<"update shard: "<<k<<" ---- src: "<<src<<" dest: "<<dest<<" value: "<<result[src - firstActiveVertex]<<endl;
					}
					else {
						srcs[k] = src;
						break;
					}
				}
			}

			// write pagerank result to stdout
			for(int i=0; i<(firstActiveVertexInNextShard - firstActiveVertex); i++) {
				int vertexID = i + firstActiveVertex;
				cout <<"vertex id: "<<vertexID<< " pagerank: "<<result[i] * graph->out_edge_counts[vertexID]<<endl;
			}

			// dealloc active shard content on host and device
			delete[] activeShardContent;
			activeShardContent = 0;
			cudaFree(activeShardContent_d);

			// dealloc result on host and device
			delete[] result;
			result = 0;
			cudaFree(result_d);

			fclose(ifActiveShard);
    	}
    	
    	// close files
    	for(int j=0; j<par->num_shards; j++) {
    		fclose(ifShards[j]);
    		fclose(tfShards[j]);
    	}

    	// rename temp files for the next iteration
    	for(int j=0; j<par->num_shards; j++) {
    		rename( ("temp"+graph->shard_fnames[j]).c_str(), (graph->shard_fnames[j]).c_str());
    	}
    }
    cout<<"end of cuda"<<endl;
}

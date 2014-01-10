#ifndef UTILITY_H
#define UTILITY_H
#include <string>

template <class T>
inline void hash_combine(std::size_t & seed, const T & v)
{
  std::hash<T> hasher;
  seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std
{
  template<typename S, typename T> struct hash<pair<S, T>>
  {
      inline size_t operator()(const pair<S, T> & v) const
      {
            size_t seed = 0;
            ::hash_combine(seed, v.first);
            ::hash_combine(seed, v.second);
            return seed;
          }
    };
}

typedef struct global_parameters {
    int num_shards;
    int mem_budget; //in MB
    int edge_mem_cost; //single edge memory cost in Bytes
    int max_num_edges; //per shard
} params;

std::string get_fname_from_path(char* filepath){
    std::string str(filepath);

    size_t found = str.rfind("/");
    //if it can be found crop string
    if (found != std::string::npos){
        return str.substr(found + 1); //start from next char
    }

    return str;
}

#endif

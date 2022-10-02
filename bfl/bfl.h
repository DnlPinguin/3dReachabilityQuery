#ifndef BFL_H
#define BFL_H

#include <sys/time.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iterator>
#include <utility>
#include <vector>
#include <string>

using namespace std;

#ifndef K
#define K 5
#endif
#ifndef D
#define D (320 * K)
#endif

struct node {
  int N_O_SZ, N_I_SZ;
  int *N_O, *N_I;
  int vis;
  union {
    int L_in[K];
#if K > 8
    unsigned int h_in;
#else
    unsigned char h_in;
#endif
  };
  union {
    int L_out[K];
#if K > 8
    unsigned int h_out;
#else
    unsigned char h_out;
#endif
  };
  pair<int, int> L_interval;
};

void read_graph_for_bfl(const char *filename);
bool run_all_bfl_queries(vector<pair<int,int>> queries);
bool run_single_bfl_query(int source, int target);
void bfl_index_construction();

#endif 
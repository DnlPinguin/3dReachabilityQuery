#include "bfl.h"
#include <iostream>
#include <fstream>
#include <set>


using namespace std;


vector<node> nodes;
int vis_cur, cur;

void read_graph_for_bfl(const char *filename) {
  timeval start_at, end_at;
  gettimeofday(&start_at, 0);
  FILE *file = fopen(filename, "r");
  char header[] = "graph_for_greach";
  fscanf(file, "%s", header);
  int n;
  fscanf(file, "%d", &n);

  nodes.resize(n);
  vector<vector<int>> N_O(n), N_I(n);
  set<int> counter;
  for (;;) {
    int u, v;
    if (feof(file) || fscanf(file, "%d", &u) != 1) {
      break;
    }
    fgetc(file);
    while (!feof(file) && fscanf(file, "%d", &v) == 1) {
      N_O[u].push_back(v);
      N_I[v].push_back(u);
      counter.insert(u);
      counter.insert(v);
    }
    fgetc(file);
  }
  fclose(file);
  

  for (int u = 0; u < n; u++) {
    nodes[u].N_O_SZ = N_O[u].size();
    nodes[u].N_O = new int[N_O[u].size()];
    copy(N_O[u].begin(), N_O[u].end(), nodes[u].N_O);
    nodes[u].N_I_SZ = N_I[u].size();
    nodes[u].N_I = new int[N_I[u].size()];
    copy(N_I[u].begin(), N_I[u].end(), nodes[u].N_I);
  }
  gettimeofday(&end_at, 0);
  printf("read time(graph): %.3fs\n",
         end_at.tv_sec - start_at.tv_sec +
             double(end_at.tv_usec - start_at.tv_usec) / 1000000);
}

int h_in() {
  static int c = 0, r = rand();
  if (c >= (int)nodes.size() / D) {
    c = 0;
    r = rand();
  }
  c++;
  return r;
}

int h_out() {
  static int c = 0, r = rand();
  if (c >= (int)nodes.size() / D) {
    c = 0;
    r = rand();
  }
  c++;
  return r;
}

void dfs_in(node &u) {
  u.vis = vis_cur;

  if (u.N_I_SZ == 0) {
    u.h_in = h_in() % (K * 32);
  } else {
    for (int i = 0; i < K; i++) {
      u.L_in[i] = 0;
    }

    for (int i = 0; i < u.N_I_SZ; i++) {
      node &v = nodes[u.N_I[i]];
      if (v.vis != vis_cur) {
        dfs_in(v);
      }
      if (v.N_I_SZ == 0) {
        int hu = v.h_in;
        u.L_in[(hu >> 5) % K] |= 1 << (hu & 31);
      } else {
        for (int j = 0; j < K; j++) {
          u.L_in[j] |= v.L_in[j];
        }
      }
    }

    int hu = h_in();
    u.L_in[(hu >> 5) % K] |= 1 << (hu & 31);
  }
}

void dfs_out(node &u) {
  u.vis = vis_cur;

  u.L_interval.first = cur++;

  if (u.N_O_SZ == 0) {
    u.h_out = h_out() % (K * 32);
  } else {
    for (int i = 0; i < K; i++) {
      u.L_out[i] = 0;
    }

    for (int i = 0; i < u.N_O_SZ; i++) {
      node &v = nodes[u.N_O[i]];
      if (v.vis != vis_cur) {
        dfs_out(v);
      }
      if (v.N_O_SZ == 0) {
        int hu = v.h_out;
        u.L_out[(hu >> 5) % K] |= 1 << (hu & 31);
      } else {
        for (int j = 0; j < K; j++) {
          u.L_out[j] |= v.L_out[j];
        }
      }
    }

    int hu = h_out();
    u.L_out[(hu >> 5) % K] |= 1 << (hu & 31);
  }

  u.L_interval.second = cur;
}

void bfl_index_construction() {
  timeval start_at, end_at;
  gettimeofday(&start_at, 0);

  vis_cur++;
  for (int u = 0; u < nodes.size(); u++) {
    if (nodes[u].N_O_SZ == 0) {
      dfs_in(nodes[u]);
    }
  }
  vis_cur++;
  cur = 0;
  for (int u = 0; u < nodes.size(); u++) {
    if (nodes[u].N_I_SZ == 0) {
      dfs_out(nodes[u]);
    }
  }

  gettimeofday(&end_at, 0);
  printf("index time: %.3fs\n",
         end_at.tv_sec - start_at.tv_sec +
             double(end_at.tv_usec - start_at.tv_usec) / 1000000);
  long long index_size = 0;
  for (int u = 0; u < nodes.size(); u++) {
    index_size +=
        nodes[u].N_I_SZ == 0 ? sizeof(nodes[u].h_in) : sizeof(nodes[u].L_in);
    index_size +=
        nodes[u].N_O_SZ == 0 ? sizeof(nodes[u].h_out) : sizeof(nodes[u].L_out);
    index_size += sizeof(nodes[u].L_interval);
  }
  // printf("index space: %.3fMB\n", double(index_size) / (1024 * 1024));
}

bool reach(node &u, node &v) {
  if (u.L_interval.second < v.L_interval.second) {
    return false;
  } else if (u.L_interval.first <= v.L_interval.first) {
    return true;
  }

  if (v.N_I_SZ == 0) {
    return false;
  }
  if (u.N_O_SZ == 0) {
    return false;
  }
  if (v.N_O_SZ == 0) {
    if ((u.L_out[v.h_out >> 5] & (1 << (v.h_out & 31))) == 0) {
      return false;
    }
  } else {
    for (int i = 0; i < K; i++) {
      if ((u.L_out[i] & v.L_out[i]) != v.L_out[i]) {
        return false;
      }
    }
  }
  if (u.N_I_SZ == 0) {
    if ((v.L_in[u.h_in >> 5] & (1 << (u.h_in & 31))) == 0) {
      return false;
    }
  } else {
    for (int i = 0; i < K; i++) {
      if ((u.L_in[i] & v.L_in[i]) != u.L_in[i]) {
        return false;
      }
    }
  }

  for (int i = 0; i < u.N_O_SZ; i++) {
    if (nodes[u.N_O[i]].vis != vis_cur) {
      nodes[u.N_O[i]].vis = vis_cur;
      if (reach(nodes[u.N_O[i]], v)) {
        return true;
      }
    }
  }
  return false;
}

bool run_single_bfl_query(int source, int target){
  if (source == target){
    return false;
  }
  vis_cur++;
  bool hit = reach(nodes[source], nodes[target]);
  return hit;
}

bool run_all_bfl_queries(vector<pair<int,int>> queries){
  for (vector<pair<int,int>>::iterator iter = queries.begin(); iter != queries.end(); iter++){
    if (reach(nodes[iter->first], nodes[iter->second])){
      return true;
    }
  }
  return false;
}






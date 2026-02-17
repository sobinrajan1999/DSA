#include <bits/stdc++.h>
using namespace std;

class UnionFind {
   private:
    vector<int> parent, rank;

   public:
    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    int find(int u) {
        if (parent[u] != u) {
            parent[u] = find(parent[u]);  // Path compression
        }
        return parent[u];
    }

    //union by rank
    void unionSets(int u, int v) {
        int rootU = find(u);
        int rootV = find(v);
        if (rootU != rootV) {
            // Union by rank
            if (rank[rootU] < rank[rootV]) {
                parent[rootU] = rootV;
            } else if (rank[rootU] > rank[rootV]) {
                parent[rootV] = rootU;
            } else {
                parent[rootV] = rootU;
                rank[rootU]++;
            }
        }
    }

    void unionSets(int u, int v, bool byRank) {
        if (byRank) {
            unionSets(u, v);
        } else {
            int rootU = find(u);
            int rootV = find(v);
            if (rootU != rootV) {
                parent[rootV] = rootU;  // Simple union without rank
            }
        }
    }

    //union by size
    void unionBySize(int u, int v, vector<int>& size) {
        int rootU = find(u);
        int rootV = find(v);
        if (rootU != rootV) {
            if (size[rootU] < size[rootV]) {
                parent[rootU] = rootV;
                size[rootV] += size[rootU];
            } else {
                parent[rootV] = rootU;
                size[rootU] += size[rootV];
            }
        }
    }

    bool connected(int u, int v) {
        return find(u) == find(v);
    }
};
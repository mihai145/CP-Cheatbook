//
// Created by Mihai145 on 4/27/2021.
//

/*
 * Test problem: https://codeforces.com/edu/course/2/lesson/7/2/practice/contest/289391/problem/J
 * Adding edges into a graph one by one, determine when the graph becomes non-bipartite.
 * DSU -> O(E * logstarV)
 */

#include <iostream>
#include <vector>
using namespace std;

struct DSU {
    vector <int> dad, sz;
    vector <bool> dist;
    void Init(int n) {
        dad.resize(n + 1);
        sz.resize(n + 1);
        dist.resize(n + 1);
        for(int i = 1; i <= n; i++) {
            dad[i] = i, sz[i] = 1, dist[i] = 0;
        }
    }
    pair <int, bool> Find(int node) {
        if(dad[node] == node) {
            return {node, 0};
        }
        pair <int, bool> ft = Find(dad[node]);
        dad[node] = ft.first, dist[node] = dist[node] ^ ft.second;
        return {dad[node], dist[node]};
    }
    bool Unite(int p, int q) {
        pair <int, bool> P = Find(p), Q = Find(q);
        if(P.first == Q.first && (P.second ^ Q.second) == 0) {
            return false;
        }
        if(sz[P.first] > sz[Q.first]) {
            swap(P, Q);
        }
        dad[P.first] = Q.first;
        sz[Q.first] += sz[P.first];
        dist[P.first] = P.second ^ 1 ^ Q.second;
        return true;
    }

};

int main() {
    int N, M; cin >> N >> M;
    DSU dsu; dsu.Init(N);
    for(int i = 1; i <= M; i++) {
        int x, y; cin >> x >> y;
        bool r = dsu.Unite(x, y);
        if(r == false) { //The graph is no longer bipartite
            cout << i << '\n';
            return 0;
        }
    }
    cout << -1 << '\n';
    return 0;
}
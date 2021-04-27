//
// Created by Mihai145 on 4/27/2021.
//

/*
 * Test problem: https://codeforces.com/contest/321/problem/C
 *
 * Given a tree with N vertices, assign letters 'A'-'Z' to each vertex such that
 * the simple path between any pair of vertices with the same letter goes through a vertex
 * that has a lexicographically smaller letter.
 *
 * Idea: Build the centroid tree of the given tree. Assign letters 'A'-'Z' to the vertices
 * according to their depth in the centroid tree (i.e. the root of the centroid tree
 * is assigned 'A', its sons are assigned 'B' and so on). As a centroid splits its subgraph
 * into disjoint parts, no two sons of a centroid will be in the same part. Hence, any centroid lies
 * on all simple paths connecting two of its sons, and so the assignment is correct.
 */

#include <iostream>
#include <vector>
using namespace std;

const int NMAX = 200000;
int N, sz[NMAX + 2], centroidTreeRoot = -1;
vector <int> g[NMAX + 2], centroidTree[NMAX + 2];
bool isCentroid[NMAX + 2];

void dfsSize(int root, int parent) {
    sz[root] = 1;
    for(int it : g[root]) {
        if (!isCentroid[it] && it != parent) {
            dfsSize(it, root);
            sz[root] += sz[it];
        }
    }
}
int FindCentroid(int root, int parent, int n) {
    if(n == 1) {
        return root;
    }
    for(int it : g[root]) {
        if (it != parent && !isCentroid[it]) {
            if (sz[it] > n / 2) {
                return FindCentroid(it, root, n);
            }
        }
    }
    return root;
}
int GetCentroid(int root) {
    dfsSize(root, -1);
    return FindCentroid(root, -1, sz[root]);
}
int Decompose(int root) {
    int centroid = GetCentroid(root);
    isCentroid[centroid] = true;
    if(centroidTreeRoot == -1) {
        centroidTreeRoot = centroid;
    }
    for(int it : g[centroid]) {
        if (!isCentroid[it]) {
            int x = Decompose(it);
            centroidTree[centroid].push_back(x), centroidTree[x].push_back(centroid);
        }
    }
    return centroid;
}

char color[NMAX + 5];
void Colour(int node, int h) {
    color[node] = h + 'A';
    for(int it : centroidTree[node]) {
        if (!color[it]) Colour(it, h + 1);
    }
}

int main() {
    cin >> N;
    for(int i = 1; i < N; i++) {
        int x, y; cin >> x >> y;
        g[x].push_back(y), g[y].push_back(x);
    }
    Decompose(1);
    Colour(centroidTreeRoot, 0);
    for(int i = 1; i <= N; i++) {
        cout << color[i] << ' ';
    }
    return 0;
}
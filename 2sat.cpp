//
// Created by Mihai145 on 4/26/2021.
//

/*
 * Test problem: https://infoarena.ro/problema/2sat
 */

#include <fstream>
#include <vector>
using namespace std;

const int NMAX = 1e5;
int N, M, seen[2 * NMAX + 2], kComp, comp[2 * NMAX + 2];
vector<int> graph[2 * NMAX + 2], rev[2 * NMAX + 2], stk;

int Norm(int x) {
    if(x > 0) { return x; }
    return -x + N;
}
int Not(int x) {
    if(x > 0) { return x + N; }
    return -x;
}
void dfs1(int node) {
    seen[node] = 1;
    for(int son : graph[node]) {
        if(!seen[son]) {
            dfs1(son);
        }
    }
    stk.push_back(node);
}
void dfs2(int node) {
    seen[node] = 0, comp[node] = kComp;
    for(int son : rev[node]) {
        if(seen[son]) {
            dfs2(son);
        }
    }
}
void AddEdge(int x, int y) {
    graph[Not(x)].push_back(Norm(y));
    rev[Norm(y)].push_back(Not(x));
}
void stronglyConnectedComponents() {
    for(int i = 1; i <= 2 * N; i++) {
        if(!seen[i]) {
            dfs1(i);
        }
    }
    while(!stk.empty()) {
        int node = stk.back(); stk.pop_back();
        if(seen[node] == true) {
            ++kComp; dfs2(node);
        }
    }
}

int main() {
    ifstream f("2sat.in"); ofstream g("2sat.out");
    f >> N >> M;
    for(int i = 1; i <= M; i++) {
        int x, y; f >> x >> y;
        AddEdge(x, y); AddEdge(y, x);
    }
    stronglyConnectedComponents();
    for(int i = 1; i <= N; i++) {
        if(comp[i] == comp[i + N]) { //No solution
            g << -1 << '\n';
            return 0;
        }
    }
    for(int i = 1; i <= N; i++) {
        if(comp[i] < comp[i + N]) { //ith variable is false
            g << 0 << ' ';
        } else {                    //ith variable is true
            g << 1 << ' ';
        }
    }
    return 0;
}
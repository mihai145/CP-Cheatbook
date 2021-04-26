//
// Created by Mihai145 on 4/26/2021.
//

/*
 * Test problem: https://infoarena.ro/problema/biconex
 */

#include <fstream>
#include <vector>
using namespace std;

const int NMAX = 1e5;
int N, M, k, t[NMAX + 2], low[NMAX + 2];
vector <int> graph[NMAX + 2], stk;
vector<vector<int>> biconnectedComponents;
void dfs(int node, int parent) {
    stk.push_back(node);
    k++, t[node] = low[node] = k;
    for(int son : graph[node]) {
        if(son != parent) {
            if(t[son] != 0) {
                low[node] = min(low[node], t[son]);
                continue;
            }
            dfs(son, node);
            low[node] = min(low[node], low[son]);
            if(t[node] <= low[son]) {
                vector<int> currentComponent;
                int currentVertex;
                do {
                    currentVertex = stk.back(), stk.pop_back();
                    currentComponent.push_back(currentVertex);
                } while(currentVertex != son);
                currentComponent.push_back(node);
                biconnectedComponents.push_back(currentComponent);
            }
        }
    }
}

int main() {
    ifstream f("biconex.in"); ofstream g("biconex.out");
    f >> N >> M;
    for(int i = 1; i <= M; i++) {
        int x, y; f >> x >> y;
        graph[x].push_back(y), graph[y].push_back(x);
    }
    dfs(1, -1);
    g << (int)biconnectedComponents.size() << '\n';
    for(vector<int> comp : biconnectedComponents) {
        for(int vertex : comp) {
            g << vertex << ' ';
        }
        g << '\n';
    }
    return 0;
}
//
// Created by Mihai145 on 4/27/2021.
//

/*
 * Test problem: https://infoarena.ro/problema/maxflow
 */

#include <fstream>
#include <queue>
using namespace std;

const int NMAX = 1000;
int N, M, capacity[NMAX + 2][NMAX + 2], flow[NMAX + 2][NMAX + 2], parent[NMAX + 2];
vector <int> graph[NMAX + 2];
bool seen[NMAX + 2];

bool BFS() {
    for(int i = 1; i <= N; i++) {
        seen[i] = false;
        parent[i] = 0;
    }
    queue <int> q; q.push(1);
    seen[1] = true;
    while(!q.empty()) {
        int node = q.front(); q.pop();
        for(int it : graph[node]) {
            if(flow[node][it] < capacity[node][it] && seen[it] == false) {
                seen[it] = true, parent[it] = node;
                q.push(it);
            }
        }
    }
    return seen[N];
}

int main() {
    ifstream f("maxflow.in"); ofstream g("maxflow.out");
    f >> N >> M;
    for(int i = 1; i <= M; i++) {
        int x, y, c; f >> x >> y >> c;
        graph[x].push_back(y), graph[y].push_back(x);
        capacity[x][y] = c;
    }
    while(BFS()) {
        for(int pv : graph[N]) {
            int pumpFlow = capacity[pv][N] - flow[pv][N];
            for(int node = pv; node != 1; node = parent[node]) {
                pumpFlow = min(pumpFlow, capacity[parent[node]][node] - flow[parent[node]][node]);
            }
            flow[pv][N] += pumpFlow, flow[N][pv] -= pumpFlow;
            for(int node = pv; node != 1; node = parent[node]) {
                flow[parent[node]][node] += pumpFlow, flow[node][parent[node]] -= pumpFlow;
            }
        }
    }
    int ans = 0;
    for(int it : graph[1]) {
        ans += flow[1][it];
    }
    g << ans << '\n';
    return 0;
}
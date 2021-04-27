//
// Created by Mihai145 on 4/27/2021.
//

/*
 * Test problem: https://infoarena.ro/problema/fmcm
 * Expected score: 70/100
 */

#include <fstream>
#include <vector>
#include <queue>
using namespace std;

const int NMAX = 350;
const int INF = 20000000;
int N, M, S, D;
vector <int> graph[NMAX + 5];
int capacity[NMAX + 5][NMAX + 5], flow[NMAX + 5][NMAX + 5], cost[NMAX + 5][NMAX + 5];
int costTo[NMAX + 5], parent[NMAX + 5], pumpFlow[NMAX + 5];
bool seen[NMAX + 5];

bool BellmanFord() {
    for(int i = 1; i <= N; i++) {
        costTo[i] = INF, pumpFlow[i] = INF;
    }
    queue <int> Q; Q.push(S);
    costTo[S] = 0, seen[S] = true;
    while(!Q.empty()) {
        int node = Q.front();
        Q.pop();
        seen[node] = false;
        for (auto it : graph[node]) {
            if (flow[node][it] < capacity[node][it]) {
                if (costTo[it] > costTo[node] + cost[node][it]) {
                    costTo[it] = costTo[node] + cost[node][it];
                    pumpFlow[it] = min(pumpFlow[node], capacity[node][it] - flow[node][it]);
                    parent[it] = node;
                    if (seen[it] == 0) {
                        seen[it] = true;
                        Q.push(it);
                    }
                }
            }
        }
    }
    return costTo[D] != INF;
}

int main() {
    ifstream f("fmcm.in"); ofstream g("fmcm.out");
    f >> N >> M >> S >> D;
    for(int i = 1; i <= M; i++) {
        int x, y, cap, cst; f >> x >> y >> cap >> cst;
        graph[x].push_back(y), graph[y].push_back(x);
        capacity[x][y] = cap;
        cost[x][y] = cst, cost[y][x] = -cst;
    }
    int ans = 0;
    while(BellmanFord()) {
        for(int i = D; i != S; i = parent[i]) {
            flow[parent[i]][i] += pumpFlow[D], flow[i][parent[i]] -= pumpFlow[D];
        }
        ans += pumpFlow[D] * costTo[D];
    }
    g << ans << '\n';
    return 0;
}
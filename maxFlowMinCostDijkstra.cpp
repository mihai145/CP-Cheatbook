//
// Created by Mihai145 on 4/27/2021.
//

/*
 * Test problem: https://infoarena.ro/problema/fmcm
 * Expected score: 100/100 O(N*M2*logN)
 * Implements Dijkstra (with potentials) after modifying the graph
 */

#include <fstream>
#include <vector>
#include <queue>
using namespace std;

const int NMAX = 350;
const int INF = 1e9;

int N, M, S, D;
vector <int> graph[NMAX + 2];
int capacity[NMAX + 2][NMAX + 2], cost[NMAX + 2][NMAX + 2], flow[NMAX + 2][NMAX + 2];
int parent[NMAX + 2], oldD[NMAX + 2], realD[NMAX + 2], d[NMAX + 2];
queue <int> Q;
bool inQ[NMAX + 2];
priority_queue <pair <int, int>, vector <pair <int, int>>, greater <pair <int, int>> > pq;

void Bellman() {
    for(int i = 1; i <= N; i++) {
        oldD[i] = INF;
    }
    Q.push(S); oldD[S] = 0; inQ[S] = true;
    while(!Q.empty()) {
        int node = Q.front(); Q.pop();
        inQ[node] = false;
        for(auto it : graph[node]) {
            if (flow[node][it] < capacity[node][it]) {
                if (oldD[it] > oldD[node] + cost[node][it]) {
                    oldD[it] = oldD[node] + cost[node][it];
                    if (!inQ[it]) {
                        Q.push(it);
                        inQ[it] = true;
                    }
                }
            }
        }
    }
}
bool Dijkstra() {
    for(int i = 1; i <= N; i++) {
        d[i] = INF;
    }
    pq.push({0, S});
    d[S] = realD[S] = 0;
    while(!pq.empty()) {
        pair <int, int> current = pq.top(); pq.pop();
        int node = current.second, cst = current.first;
        if(d[node] != cst) { continue; }
        for(auto it : graph[node]) {
            if (flow[node][it] < capacity[node][it]) {
                int distToIt = d[node] + cost[node][it] + oldD[node] - oldD[it];
                if (distToIt < d[it]) {
                    d[it] = distToIt, parent[it] = node;
                    realD[it] = realD[node] + cost[node][it];
                    pq.push({d[it], it});
                }
            }
        }
    }
    for(int i = 1; i <= N; i++) {
        oldD[i] = realD[i];
    }
    return (d[D] != INF);
}

int main() {
    ifstream f("fmcm.in"); ofstream g("fmcm.out");
    f >> N >> M >> S >> D;
    for(int i = 1; i <= M; i++) {
        int x, y, c, z; f >> x >> y >> c >> z;
        graph[x].push_back(y), graph[y].push_back(x);
        capacity[x][y] = c;
        cost[x][y] = z, cost[y][x] = -z;
    }
    int minCost = 0;
    Bellman();
    while(Dijkstra()) {
        int pumpFlow = INF;
        for(int node = D; node != S; node = parent[node]) {
            pumpFlow = min(pumpFlow, capacity[parent[node]][node] - flow[parent[node]][node]);
        }
        for(int node = D; node != S; node = parent[node]) {
            flow[parent[node]][node] += pumpFlow, flow[node][parent[node]] -= pumpFlow;
        }
        minCost += pumpFlow * realD[D];
    }
    g << minCost << '\n';
    return 0;
}
//
// Created by Mihai145 on 4/26/2021.
//

/*
 * Test problem: https://infoarena.ro/problema/hamilton
 * Finds a hamiltonian cycle of minimum cost or determines there is no hamiltonian cycle
 */

#include <fstream>
#include <vector>
using namespace std;

const int NMAX = 18;
const int MASK = (1 << NMAX);
const int INF = 1e9;
int N, M, dp[MASK + 2][NMAX + 2], minAns = INF;
vector <pair<int,int>> graph[NMAX + 2];

int main() {
    ifstream f("hamilton.in"); ofstream g("hamilton.out");
    f >> N >> M;
    for(int i = 1; i <= M; i++) {
        int x, y, c; f >> x >> y >> c;
        graph[y].push_back({x, c});
    }
    for(int i = 0; i < (1 << N); i++) {
        for(int j = 0; j < N; j++) {
            dp[i][j] = INF;
        }
    }
    dp[1][0] = 0; //dp[mask][j] -> minimum cost of visiting all marked vertices in mask, the last visited vertex being j
    for(int mask = 2; mask < (1 << N); mask++) {
        for(int j = 0; j < N; j++) {
            if(mask & (1 << j)) {
                for(pair <int, int> it : graph[j]) {
                    if(mask & (1 << it.first)) {
                        dp[mask][j] = min(dp[mask][j], dp[mask ^ (1 << j)][it.first] + it.second);
                    }
                }
            }
        }
    }
    for(pair <int, int> it : graph[0]) {
        minAns = min(minAns, dp[(1 << N) - 1][it.first] + it.second);
    }
    if(minAns == INF) { //No solution
        g << "Nu exista solutie\n";
    } else {
        g << minAns << '\n';
    }
    return 0;
}
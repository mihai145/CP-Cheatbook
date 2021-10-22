//
// Created by Mihai145 on 10/22/2021.
//

/*
 * Test problem: https://www.infoarena.ro/problema/sortaret
 * O(N + M)
 */

#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;
ifstream cin("sortaret.in");
ofstream cout("sortaret.out");

const int NMAX = 5e4;
int N, M;
vector<int> g[NMAX + 2], topSort;

bool seen[NMAX + 2];
void dfs(int node) {
    seen[node] = true;
    for (int it : g[node]) {
        if (!seen[it]) {
            dfs(it);
        }
    }
    topSort.push_back(node);
}

int main() {
    cin >> N >> M;
    for (int i = 1; i <= M; i++) {
        int x, y; cin >> x >> y;
        g[x].push_back(y);
    }
    for (int i = 1; i <= N; i++) {
        if (!seen[i]) {
            dfs(i);
        }
    }
    reverse(topSort.begin(), topSort.end());
    for (int it : topSort) {
        cout << it << ' ';
    }
    return 0;
}
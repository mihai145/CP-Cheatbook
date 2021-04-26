//
// Created by Mihai145 on 4/26/2021.
//

/*
 * Test problem: https://infoarena.ro/problema/cuplaj
 * Hopcroft-Karp O(E*sqrtV)
 */

#include <fstream>
#include <vector>
using namespace std;

const int NMAX = 1e4;
int N, M, E, matching, l[NMAX + 2], r[NMAX + 2];
vector<int> graph[NMAX + 2];
bool d[NMAX + 2];
bool PairUp(int node) {
    if(d[node]) {
        return false;
    }
    d[node] = true;
    for(int it : graph[node]) {
        if(!r[it]) {
            ++matching;
            r[it] = node, l[node] = it;
            return true;
        }
    }
    for(int it : graph[node]) {
        if(PairUp(r[it])) {
            r[it] = node, l[node] = it;
            return true;
        }
    }
    return false;
}

int main() {
    ifstream f("cuplaj.in"); ofstream g("cuplaj.out");
    f >> N >> M >> E;
    for(int i = 1; i <= E; i++) {
        int x, y; f >> x >> y;
        graph[x].push_back(y);
    }
    bool ok = false;
    do {
        ok = false;
        for(int i = 1; i <= N; i++) {
            d[i] = false;
        }
        for(int i = 1; i <= N; i++) {
            if(!l[i]) {
                ok |= PairUp(i);
            }
        }
    } while (ok);
    g << matching << '\n';
    for(int i = 1; i <= N; i++) {
        if(l[i]) {
            g << i << ' ' << l[i] << '\n';
        }
    }
    return 0;
}
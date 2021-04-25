//
// Created by Mihai145 on 4/25/2021.
//

/*
 * Test problem : https://infoarena.ro/problema/ciclueuler
 * Returns a list of vertices that form an eulerian cycle
 */

#include <fstream>
#include <vector>
#include <stack>
using namespace std;

const int NMAX = 1e5;
const int MMAX = 5e5;
int N, M, deg[NMAX + 2];
vector <pair<int, int>> graph[NMAX + 2];
bool seen[MMAX + 2];
int main() {
    ifstream f("ciclueuler.in"); ofstream g("ciclueuler.out");
    f >> N >> M;
    for(int i = 1; i <= M; i++) {
        int x, y; f >> x >> y;
        graph[x].push_back({y, i}), graph[y].push_back({x, i});
        deg[x]++, deg[y]++;
    }
    for(int i = 1; i <= N; i++) {
        if(deg[i] % 2 == 1) { //No eulerian cycle
            g << -1 << '\n';
            return 0;
        }
    }
    stack <int> st; st.push(1);
    vector <int> sol;
    while(!st.empty()) {
        int currentVertex = st.top();
        while(!graph[currentVertex].empty() && seen[graph[currentVertex].back().second]) {
            graph[currentVertex].pop_back();
        }
        if(!graph[currentVertex].empty()) {
            seen[graph[currentVertex].back().second] = true;
            st.push(graph[currentVertex].back().first);
            graph[currentVertex].pop_back();
        } else {
            sol.push_back(currentVertex);
            st.pop();
        }
    }
    for(int i = 0; i < (int)sol.size() - 1; i++) {
        g << sol[i] << ' ';
    }
    return 0;
}
//
// Created by Mihai145 on 10/22/2021.
//

/*
 * Test problem: https://www.infoarena.ro/problema/ctc
 * Kosaraju: O(N + M)
 */

#include <fstream>
#include <vector>
#include <stack>

using namespace std;
ifstream cin("ctc.in");
ofstream cout("ctc.out");

const int NMAX = 1e5;
int N, M;
vector<int> g[NMAX + 5], r[NMAX + 5];

bool seen[NMAX + 5];
stack<int> st;
void dfs(int node) {
    seen[node] = true;
    for (int it : g[node]) {
        if (!seen[it]) {
            dfs(it);
        }
    }
    st.push(node);
}

vector<int> currentCtc;
vector<vector<int>> ctc;
void dfs2(int node) {
    currentCtc.push_back(node);
    seen[node] = false;
    for (int it : r[node]) {
        if (seen[it]) {
            dfs2(it);
        }
    }
}

int main() {
    cin >> N >> M;
    for (int i = 1; i <= M; i++) {
        int x, y;
        cin >> x >> y;
        g[x].push_back(y);
        r[y].push_back(x);
    }
    for (int i = 1; i <= N; i++) {
        if (!seen[i]) {
            dfs(i);
        }
    }
    while (!st.empty()) {
        int node = st.top();
        st.pop();
        if (seen[node]) {
            currentCtc.clear();
            dfs2(node);
            ctc.push_back(currentCtc);
        }
    }
    cout << (int) ctc.size() << '\n';
    for (vector<int> comp : ctc) {
        for (int node : comp) {
            cout << node << ' ';
        }
        cout << '\n';
    }
    return 0;
}
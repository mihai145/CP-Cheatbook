//
// Created by Mihai145 on 4/27/2021.
//

/*
 * Test problem: https://infoarena.ro/problema/heavypath
 * Given a tree with N vertices with values v[1], v[2],..., v[N], answer the following queries:
 * Change v[i] = k -> O(logN)
 * Determine the biggest value on the simple path between a and b -> O(log^2)
 */

#include <fstream>
#include <vector>
using namespace std;

const int NMAX = 1e5;
int N, Q, value[NMAX + 2], dad[NMAX + 2], w[NMAX + 2], h[NMAX + 2];
vector <int> graph[NMAX + 2];
void dfs(int node, int parent) {
    dad[node] = parent, w[node] = 1;
    for(int son : graph[node]) {
        if(son != parent) {
            h[son] = h[node] + 1;
            dfs(son, node);
            w[node] += w[son];
        }
    }
}

vector <int> order;
int pos[NMAX + 2], head[NMAX + 2];
void dfs2(int node) {
    order.push_back(node), pos[node] = (int)order.size();
    if((int)graph[node].size() == 1 && node != 1) {
        return ;
    }
    int heavySon = (graph[node][0] != dad[node]) ? graph[node][0] : graph[node][1];
    for(int son : graph[node]) {
        if(son != dad[node]) {
            if(w[son] > w[heavySon]) {
                heavySon = son;
            }
        }
    }
    head[heavySon] = head[node];
    dfs2(heavySon);
    for(int son : graph[node]) {
        if(son != dad[node] && son != heavySon) {
            dfs2(son);
        }
    }
}

struct SegmentTree{
    int v[4 * NMAX];
    void Update(int node, int st, int dr, int pos, int val) {
        if(st == dr) {
            v[node] = val;
            return ;
        }
        int mid = (st + dr) >> 1;
        if(pos <= mid) {
            Update(2 * node, st, mid, pos, val);
        } else {
            Update(2 * node + 1, mid + 1, dr, pos, val);
        }
        v[node] = max(v[2 * node], v[2 * node + 1]);
    }

    int Query(int node, int st, int dr, int L, int R) {
        if(R < st || dr < L) {
            return 0;
        }
        if(L <= st && dr <= R) {
            return v[node];
        }
        int mid = (st + dr) >> 1;
        return max(Query(2 * node, st, mid, L, R),
                   Query(2 * node + 1, mid + 1, dr, L, R));
    }
};
SegmentTree st;

int Query(int x, int y) {
    if(head[x] == head[y]) { //x and y are on the same heavy chain
        if(pos[x] > pos[y]) {
            swap(x, y);
        }
        return st.Query(1, 1, N, pos[x], pos[y]);
    }
    if(h[head[x]] < h[head[y]]) {
        swap(x, y);
    }
    int ans1 = st.Query(1, 1, N, pos[head[x]], pos[x]);
    x = dad[head[x]];
    return max(ans1, Query(x, y));
}

int main() {
    ifstream f("heavypath.in"); ofstream g("heavypath.out");
    f >> N >> Q;
    for(int i = 1; i <= N; i++) {
        f >> value[i];
    }
    for(int i = 1; i < N; i++) {
        int x, y; f >> x >> y;
        graph[x].push_back(y), graph[y].push_back(x);
    }
    dfs(1, 0);

    for(int i = 1; i <= N; i++) {
        head[i] = i;
    }
    dfs2(1);

    for(int i = 1; i <= N; i++) {
        st.Update(1, 1, N, pos[i], value[i]);
    }
    for(int i = 1; i <= Q; i++) {
        int t, x, y; f >> t >> x >> y;
        if(t == 0) {
            st.Update(1, 1, N, pos[x], y);
        } else {
            g << Query(x, y) << '\n';
        }
    }
    return 0;
}
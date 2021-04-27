//
// Created by Mihai145 on 4/27/2021.
//

/*
 * Test problem: https://codeforces.com/edu/course/2/lesson/7/3/practice/contest/289392/problem/C
 * "+ u v" -> add edge u, v
 * "- u v" -> remove edge u, v
 * "?" -> calculate the number of connected components
 * O(M * logM * logN)
 */

#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector <int > dad, sz;
    stack <int > chk;
    stack <pair<pair<int,int>, pair<int,int>> > st;
    int components;
    void Init(int n) {
        dad.resize(n + 1);
        sz.resize(n + 1);
        components = n;
        for(int i = 1; i <= n; i++) {
            dad[i] = i;
            sz[i] = 1;
        }
    }
    int Find(int node) { //no path compression because of rollbacks
        if(node == dad[node]) {
            return node;
        }
        return Find(dad[node]);
    }
    void Unite(int p, int q) {
        p = Find(p), q = Find(q);
        if(p == q) {
            return ;
        }
        if(sz[p] > sz[q]) {
            swap(p, q);
        }
        st.push({{p, dad[p]}, {q, components}});
        dad[p] = q, sz[q] += sz[p], components--;
    }
    void SetCheckpoint() {
        chk.push((int)st.size());
    }
    void Rollback() {
        int timeStamp = chk.top(); chk.pop();
        while((int)st.size() > timeStamp) {
            pair<pair<int,int>, pair<int,int>> prevState = st.top(); st.pop();
            dad[prevState.first.first] = prevState.first.second;
            sz[prevState.second.first] -= sz[prevState.first.first];
            components = prevState.second.second;
        }
    }
} dsu;

struct Qr {
    int l, r, u, v;
};
map < pair<int, int>, int > openSegm;
int ans[300005];

void Solve(int L, int R, vector <Qr> &queries) {
    if(L > R) {
        return;
    }
    if(L == R) {
        for(auto it : queries) {
            if (it.l <= L && it.r >= R) {
                dsu.Unite(it.u, it.v);
            }
        }
        ans[L] = dsu.components;
        return ;
    }
    vector <Qr> usefulQueries;
    for(Qr it : queries) {
        if (it.l <= L && it.r >= R) {
            dsu.Unite(it.u, it.v);
        }
        else {
            usefulQueries.push_back(it);
        }
    }
    int mid = (L + R) >> 1;

    dsu.SetCheckpoint();
    vector <Qr> recursiveQueries;
    for(auto it : usefulQueries) {
        if((it.l <= mid && it.r >= L) || (it.r >= L && it.l <= mid)) {
            recursiveQueries.push_back(it);
        }
    }
    Solve(L, mid, recursiveQueries);
    dsu.Rollback();

    dsu.SetCheckpoint();
    recursiveQueries.clear();
    for(auto it : usefulQueries) {
        if((it.l <= R && it.r >= mid + 1) || (it.r >= mid + 1 && it.l <= R)) {
            recursiveQueries.push_back(it);
        }
    }
    Solve(mid + 1, R, recursiveQueries);
    dsu.Rollback();
}

int main() {
    ios_base::sync_with_stdio(false); cin.tie(nullptr);
    int N, Q; cin >> N >> Q;
    dsu.Init(N);
    vector <Qr> queries;
    vector <int> questions;
    for(int i = 1; i <= Q; i++) {
        char t; cin >> t;
        int u, v;
        if(t == '+') {
            cin >> u >> v;
            if(u > v) { swap(u, v); }
            openSegm[ {u,v}] = i;
        }
        else if(t == '-') {
            cin >> u >> v;
            if(u > v) { swap(u, v); }
            queries.push_back({openSegm[{u,v}], i, u, v});
            openSegm[ {u,v}] = -1;
        }
        else {
            questions.push_back(i);
        }
    }
    for(auto it : openSegm) {
        if (it.second != -1) {
            queries.push_back({it.second, Q, it.first.first, it.first.second});
        }
    }
    Solve(1, Q, queries);
    for(int it : questions) {
        cout << ans[it] << '\n';
    }
    return 0;
}
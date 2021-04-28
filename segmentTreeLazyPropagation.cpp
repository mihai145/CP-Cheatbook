//
// Created by Mihai145 on 4/28/2021.
//

/*
 * Test problem: https://codeforces.com/edu/course/2/lesson/5/4/practice/contest/280801/problem/A
 * Operations:
 * Range Set - O(logN)
 * Range Add - O(logN)
 * Range Sum - O(logN)
 */

#include <iostream>
using namespace std;

const int NMAX = 1e5;
struct SegmentTree {
    long long v[4 * NMAX];
    pair<bool, long long> lazy[4 * NMAX];
    void build(int node, int st, int dr) {
        lazy[node].second = -1;
        if(st == dr) { return; }
        int mid = (st + dr) >> 1;
        build(2 * node, st, mid);
        build(2 * node + 1, mid + 1, dr);
    }
    void propagate(int node, int st, int dr) {
        if(lazy[node].second != -1) {
            if(lazy[node].first == 0) {
                if(st != dr) {
                    lazy[2 * node] = lazy[node], lazy[2 * node + 1] = lazy[node];
                }
                v[node] = lazy[node].second * (dr - st + 1);
                lazy[node] = {0, -1};
            } else {
                if(st != dr) {
                    if(lazy[2 * node].second != -1) { lazy[2 * node].second += lazy[node].second; }
                    else { lazy[2 * node] = lazy[node]; }
                    if(lazy[2 * node + 1].second != -1) { lazy[2 * node + 1].second += lazy[node].second; }
                    else { lazy[2 * node + 1] = lazy[node]; }
                }
                v[node] += lazy[node].second * (dr - st + 1);
                lazy[node] = {0, -1};
            }
        }
    }
    void rangeSet(int node, int st, int dr, int L, int R, int value) {
        propagate(node, st, dr);
        if(R < st || dr < L) { return; }
        if(L <= st && dr <= R) {
            lazy[node].first = 0, lazy[node].second = value;
            propagate(node, st, dr);
            return;
        }
        int mid = (st + dr) >> 1;
        rangeSet(2 * node, st, mid, L, R, value);
        rangeSet(2 * node + 1, mid + 1, dr, L, R, value);
        v[node] = v[2 * node] + v[2 * node + 1];
    }
    void rangeAdd(int node, int st, int dr, int L, int R, int value) {
        propagate(node, st, dr);
        if(R < st || dr < L) { return; }
        if(L <= st && dr <= R) {
            lazy[node].first = 1, lazy[node].second = value;
            propagate(node, st, dr);
            return;
        }
        int mid = (st + dr) >> 1;
        rangeAdd(2 * node, st, mid, L, R, value);
        rangeAdd(2 * node + 1, mid + 1, dr, L, R, value);
        v[node] = v[2 * node] + v[2 * node + 1];
    }
    long long rangeSum(int node, int st, int dr, int L, int R) {
        propagate(node, st, dr);
        if(R < st || dr < L) { return 0; }
        if(L <= st && dr <= R) { return v[node]; }
        int mid = (st + dr) >> 1;
        return rangeSum(2 * node, st, mid, L, R) + rangeSum(2 * node + 1, mid + 1, dr, L, R);
    }
}sg;

int main() {
    int N, M; cin >> N >> M; sg.build(1, 1, N);
    for(int i = 1; i <= M; i++) {
        int t, l, r, v; cin >> t >> l >> r; l++;
        if(t == 1) {
            cin >> v;
            sg.rangeSet(1, 1, N, l, r, v);
        } else if(t == 2) {
            cin >> v;
            sg.rangeAdd(1, 1, N, l, r, v);
        } else {
            cout << sg.rangeSum(1, 1, N, l, r) << '\n';
        }
    }
    return 0;
}
//
// Created by Mihai145 on 11/19/2021.
//
 
/*
 * Test problem: https://www.infoarena.ro/problema/euro
 * Complexity: O(NlogN)
 */

#include <fstream>
using namespace std;
ifstream cin("euro.in");
ofstream cout("euro.out");
 
const int NMAX = 35000;

struct Line {
    long long m, n;
    long long eval(int x) const {
        return m * x + n;
    }
} segm_tree[4 * NMAX];
void insert_line(int node, int l, int r, Line ins) {
    if(l == r) {
        if(ins.eval(l) > segm_tree[node].eval(l)) {
            segm_tree[node] = ins;
        }
        return;
    }
    int mid = (l + r) >> 1;
    bool better_left = ins.eval(l) > segm_tree[node].eval(l),
        better_mid = ins.eval(mid) > segm_tree[node].eval(mid);
    if(better_mid) {
        swap(segm_tree[node], ins);
    }
    if(better_left != better_mid) {
        insert_line(node << 1, l, mid, ins);
    } else {
        insert_line((node << 1) + 1, mid + 1, r, ins);
    }
}
long long query(int node, int l, int r, int x) {
    if(l == r) {
        return segm_tree[node].eval(x);
    }
    int mid = (l + r) >> 1;
    long long candidate = 0;
    if(x <= mid) {
        candidate = query(node << 1, l, mid, x);
    } else {
        candidate = query((node << 1) + 1, mid + 1, r, x);
    }
    return max(segm_tree[node].eval(x), candidate);
}
 
int main() {
    int N, T;
    long long sp[NMAX + 2] = {0LL}, dp[NMAX + 2] = {0LL};
    cin >> N >> T;
    for(int i = 1; i <= N; i++) {
        cin >> sp[i];
        sp[i] += sp[i - 1];
    }
 
    insert_line(1, 1, N, {0, 0});
    for(int i = 1; i <= N; i++) {
        dp[i] = sp[i] * i - T + query(1, 1, N, i);
        insert_line(1, 1, N, {-sp[i], dp[i]});
    }
 
    cout << dp[N] << '\n';
    return 0;
}
//
// Created by Mihai145 on 4/5/2022.
//

/*
 * Test problem: https://www.infoarena.ro/problema/lca
 * O(N) preprocess, O(1) query
 */

#include <fstream>
#include <vector>

using namespace std;

ifstream cin("lca.in");
ofstream cout("lca.out");

const int NMAX = 1e5;
int N, Q;
vector<int> g[NMAX + 2];

int level[NMAX + 2], parent[NMAX + 2];
int k_preorder, preorder[NMAX + 2], sz[NMAX + 2];
int inlabel[NMAX + 2], ascendant[NMAX + 2], head[NMAX + 2];

inline int floor_log2(const int& x) {
    return 31 - __builtin_clz(x);
}

inline int lsb(const int& x) {
    return x & -x;
}

void dfs_preorder(const int node, const int p = -1) {
    parent[node] = p;
    preorder[node] = ++k_preorder;
    sz[node] = 1;

    for (const int &son: g[node]) {
        level[son] = level[node] + 1;
        dfs_preorder(son, node);
        sz[node] += sz[son];
    }

    int i = floor_log2((preorder[node] - 1) ^ (preorder[node] + sz[node] - 1));
    inlabel[node] = ((preorder[node] + sz[node] - 1) >> i) << i;
}

void dfs_ascendant(const int node, const int p = -1) {
    if (p != -1) {
        if (inlabel[node] == inlabel[p]) {
            ascendant[node] = ascendant[p];
        } else {
            ascendant[node] = ascendant[p] + (1 << floor_log2(lsb(inlabel[node])));
        }
    }

    if (p == -1 || inlabel[node] != inlabel[p]) {
        head[inlabel[node]] = node;
    }

    for (const int &son: g[node]) {
        dfs_ascendant(son, node);
    }
}

void schieber_vishkin() {
    level[1] = 0;
    dfs_preorder(1);

    ascendant[1] = (1 << floor_log2(N));
    dfs_ascendant(1);
}

int query(int x, int y) {
    if (inlabel[x] == inlabel[y]) {
        return (level[x] <= level[y]) ? x : y;
    }

    /* Compute inlabel[z] */
    int i = max(
            floor_log2(inlabel[x] ^ inlabel[y]),
            max(
                    floor_log2(lsb(inlabel[x])),
                    floor_log2(lsb(inlabel[y]))
            )
    );

    int common = ascendant[x] & ascendant[y];
    int common_i = (common >> i) << i;
    int j = floor_log2(lsb(common_i));

    int inlabel_z = ((inlabel[x] >> (j + 1)) << (j + 1)) | (1 << j);

    /* Compute xp */
    int xp = x;
    if (inlabel[x] != inlabel_z) {
        int k = floor_log2(ascendant[x] & ((1 << j) - 1));
        int inlabel_w = ((inlabel[x] >> (k + 1)) << (k + 1)) | (1 << k);
        int w = head[inlabel_w];
        xp = parent[w];
    }

    /* Compute yp */
    int yp = y;
    if (inlabel[y] != inlabel_z) {
        int k = floor_log2(ascendant[y] & ((1 << j) - 1));
        int inlabel_w = ((inlabel[y] >> (k + 1)) << (k + 1)) | (1 << k);
        int w = head[inlabel_w];
        yp = parent[w];
    }

    return (level[xp] <= level[yp]) ? xp : yp;
}

int main() {
    cin >> N >> Q;
    for (int i = 2; i <= N; i++) {
        int t; cin >> t;
        g[t].push_back(i);
    }

    schieber_vishkin();

    for (int i = 1; i <= Q; i++) {
        int x, y; cin >> x >> y;
        cout << query(x, y) << '\n';
    }

    return 0;
}
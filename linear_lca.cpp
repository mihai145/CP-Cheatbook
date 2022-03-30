//
// Created by Mihai145 on 3/30/2022.
//

/*
 * Test problem: https://www.infoarena.ro/problema/lca
 * O(N) build, O(1) query
 */

#include <fstream>
#include <vector>
#include <stack>

const int NMAX = 1e5;
const int BUCKET_SZ = 32;
const int INF = 2e9;

std::ifstream cin("lca.in");
std::ofstream cout("lca.out");

int N, Q;
std::vector<int> g[NMAX + 2];

int k_euler, euler[2 * NMAX + 2];
int h[NMAX + 2], first_ap[NMAX + 2];

/* leftmost position of the leftmost bucket that starts after the position i - 1 */
int next_bucket[2 * NMAX + 2];
/* rightmost position of the rightmost bucket that starts before the position i + 1 */
int prev_bucket[2 * NMAX + 2];
int bitmask[2 * NMAX + 2];
int compressed_rmq[BUCKET_SZ][((2 * NMAX) / BUCKET_SZ) + 2];

void build() {
    std::stack<int> asc_stack;
    int running_minimum = INF;

    for (int i = 0; i < k_euler; i++) {
        if (running_minimum == INF || h[running_minimum] > h[euler[i]]) {
            running_minimum = euler[i];
        }

        next_bucket[i] = i - i % BUCKET_SZ + BUCKET_SZ;
        prev_bucket[i] = i - i % BUCKET_SZ - 1;

        if (i % BUCKET_SZ == 0) {
            next_bucket[i] = i;
            running_minimum = euler[i];
        } else if (i % BUCKET_SZ == BUCKET_SZ - 1) {
            prev_bucket[i] = i;
            compressed_rmq[0][i / BUCKET_SZ] = running_minimum;
        }

        while (!asc_stack.empty() && i - asc_stack.top() < BUCKET_SZ && h[euler[asc_stack.top()]] >= h[euler[i]]) {
            asc_stack.pop();
        }

        int mask = 0;
        if (!asc_stack.empty() && i - asc_stack.top() < BUCKET_SZ && h[euler[asc_stack.top()]] < h[euler[i]]) {
            mask = bitmask[asc_stack.top()] << (i - asc_stack.top());
        }
        bitmask[i] = mask | 1;
        asc_stack.push(i);
    }

    int N_compressed = (k_euler - 1) / BUCKET_SZ;
    compressed_rmq[0][k_euler] = running_minimum;

    /* build compressed rmq */
    for (int i = 1; i < BUCKET_SZ; i++) {
        if ((1 << i) > N_compressed + 1) break;
        for (int j = 0; j <= N_compressed; j++) {
            if (j + (1 << i) > N_compressed + 1) break;
            else compressed_rmq[i][j] = std::min(compressed_rmq[i - 1][j], compressed_rmq[i - 1][j + (1 << (i - 1))]);
        }
    }
}

inline int bucket_query(int l, int r) {
    if (l > r) return INF;
    int truncated_bitmask = bitmask[r] & ((1 << (r - l + 1)) - 1);
    int msb = BUCKET_SZ - __builtin_clz(truncated_bitmask) - 1; //msb(x) = floor(log2(x))
    return euler[r - msb];
}

inline int compressed_query(int l, int r) {
    if (l > r) return INF;
    int k = BUCKET_SZ - __builtin_clz(r - l + 1) - 1; //k = floor(log2(r - l + 1))
    return std::min(compressed_rmq[k][l], compressed_rmq[k][r - (1 << k) + 1]);
}

inline int query_min(int l, int r) {
    if (l > r) std::swap(l, r);
    return std::min(
            bucket_query(l, std::min(next_bucket[l] - 1, r)),
            std::min(compressed_query(next_bucket[l] / BUCKET_SZ, prev_bucket[r] < 0 ? -1 : prev_bucket[r] / BUCKET_SZ),
                     bucket_query(std::max(l, prev_bucket[r] + 1), r))
    );
}

void dfs(int node, int parent = -1) {
    euler[k_euler] = node;
    first_ap[node] = k_euler;
    k_euler++;

    for (const int &son: g[node]) {
        if (son != parent) {
            h[son] = h[node] + 1;
            dfs(son, node);
            euler[k_euler] = node;
            k_euler++;
        }
    }
}

int main() {
    cin >> N >> Q;
    for (int i = 2; i <= N; i++) {
        int t;
        cin >> t;
        g[t].push_back(i);
    }

    h[1] = 1;
    dfs(1);
    build();

    for (int i = 1; i <= Q; i++) {
        int x, y;
        cin >> x >> y;
        cout << query_min(first_ap[x], first_ap[y]) << '\n';
    }

    return 0;
}
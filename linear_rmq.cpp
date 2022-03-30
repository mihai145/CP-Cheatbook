//
// Created by Mihai145 on 3/30/2021.
//

/*
 * Test problem: https://infoarena.ro/problema/rmq
 * O(N) build, O(1) query
 */

#include <fstream>
#include <stack>

const int NMAX = 1e5;
const int BUCKET_SZ = 32;
const int INF = 2e9;

std::ifstream cin("rmq.in");
std::ofstream cout("rmq.out");

int N, Q, a[NMAX + 2];

/* leftmost position of the leftmost bucket that starts after the position i - 1 */
int next_bucket[NMAX + 2];
/* rightmost position of the rightmost bucket that starts before the position i + 1 */
int prev_bucket[NMAX + 2];
int bitmask[NMAX + 2];
int compressed_rmq[BUCKET_SZ][(NMAX / BUCKET_SZ) + 2];

void build() {
    std::stack<int> asc_stack;
    int running_minimum = INF;

    for (int i = 0; i < N; i++) {
        running_minimum = std::min(running_minimum, a[i]);

        next_bucket[i] = i - i % BUCKET_SZ + BUCKET_SZ;
        prev_bucket[i] = i - i % BUCKET_SZ - 1;

        if (i % BUCKET_SZ == 0) {
            next_bucket[i] = i;
            running_minimum = a[i];
        } else if (i % BUCKET_SZ == BUCKET_SZ - 1) {
            prev_bucket[i] = i;
            compressed_rmq[0][i / BUCKET_SZ] = running_minimum;
        }

        while (!asc_stack.empty() && i - asc_stack.top() < BUCKET_SZ && a[asc_stack.top()] >= a[i]) {
            asc_stack.pop();
        }

        int mask = 0;
        if (!asc_stack.empty() && i - asc_stack.top() < BUCKET_SZ && a[asc_stack.top()] < a[i]) {
            mask = bitmask[asc_stack.top()] << (i - asc_stack.top());
        }
        bitmask[i] = mask | 1;
        asc_stack.push(i);
    }

    int N_compressed = (N - 1) / BUCKET_SZ;
    compressed_rmq[0][N_compressed] = running_minimum;

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
    return a[r - msb];
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

int main() {
    cin >> N >> Q;
    for (int i = 0; i < N; i++) {
        cin >> a[i];
    }
    build();

    for (int i = 1; i <= Q; i++) {
        int x, y;
        cin >> x >> y;
        x--, y--;
        cout << query_min(x, y) << '\n';
    }

    return 0;
}

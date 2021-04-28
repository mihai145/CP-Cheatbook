//
// Created by Mihai145 on 4/28/2021.
//

/*
 * Test problem: https://infoarena.ro/problema/cubeon
 * O(N^2 * K) -> O(N * logN * K)
 */

#include <fstream>
using namespace std;

const int NMAX = 1000000;
const long long INF = (1LL << 60);
int N, K, sp[NMAX + 5];
long long dp[2][NMAX + 5];
long long Cube(int x) {
    return 1LL * x * x * x;
}
void Solve(int l, int st, int dr, int optSt, int optDr) {
    if(st > dr) {
        return;
    }
    int mid = (st + dr) >> 1, opt = mid;
    dp[l][mid] = INF;
    for(int i = max(0, optSt); i <= min(mid, optDr); i++) {
        long long f = dp[!l][i] + Cube(sp[mid] - sp[i]);
        if(f < dp[l][mid]) {
            opt = i;
            dp[l][mid] = f;
        }
    }
    Solve(l, st, mid - 1, optSt, opt);
    Solve(l, mid + 1, dr, opt, optDr);
}

int main() {
    ifstream f("cubeon.in"); ofstream g("cubeon.out");
    int T; f >> T;
    while(T--) {
        f >> N >> K;
        for(int i = 1; i <= N; i++) {
            f >> sp[i];
            sp[i] += sp[i - 1];
        }
        for(int i = 0; i <= N; i++) {
            dp[0][i] = dp[1][i] = INF;
        }
        for(int i = 1; i <= N; i++) {
            dp[1][i] = Cube(sp[i]);
        }
        for(int i = 2; i <= K; i++) {
            Solve(i & 1, 1, N, 0, N);
        }
        g << dp[K & 1][N] << '\n';
    }
    return 0;
}
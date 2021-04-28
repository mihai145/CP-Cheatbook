//
// Created by Mihai145 on 4/28/2021.
//

/*
 * Test problem: https://www.infoarena.ro/problema/permutariab
 * Minimum number of swaps to turn one permutation into another
 */

#include <iostream>
#include <fstream>
#pragma GCC optimize("unroll-loops")
using namespace std;

const int NMAX = 1e5;
int N, a[NMAX + 2], where[NMAX + 2], aib[NMAX + 2];
void Update(int pos) {
    for(int i = pos; i <= N; i += (i & (-i))) {
        aib[i] += 1;
    }
}
int Query(int pos) {
    int ans = 0;
    for(int i = pos; i > 0; i -= (i & (-i))) {
        ans += aib[i];
    }
    return ans;
}

int main() {
    ifstream f("permutariab.in"); ofstream g("permutariab.out");
    f >> N;
    for(int i = 1; i <= N; i++) {
        f >> a[i];
    }
    for(int i = 1; i <= N; i++) {
        int x; f >> x;
        where[x] = i;
    }
    long long sol = 0;
    for(int i = N; i >= 1; i--) {
        sol += (1LL * i - where[a[i]] + Query(where[a[i]] - 1));
        Update(where[a[i]]);
    }
    g << sol << '\n';
    return 0;
}

//
// Created by Mihai145 on 4/25/2021.
//

/*
 * Test problem : https://infoarena.ro/problema/aib
 * Point update O(logN) : v[a] += b
 * Range Query O(logN) : sum(v[l]...v[r])
 * Prefix search O(logN) : minimum k such that sum(v[1]...v[k]) == s
 */

#include <fstream>
using namespace std;

const int NMAX = 1e5;
int N, Q, fenwick[NMAX + 5];
void pointUpdate(int pos, int val) {
    for(int i = pos; i <= N; i += i & (-i)) {
        fenwick[i] += val;
    }
}
int prefixSum(int pos) {
    int ans = 0;
    for(int i = pos; i > 0; i -= i & (-i)) {
        ans += fenwick[i];
    }
    return ans;
}
int prefixSearch(int s)
{
    int k = 0;
    for(int i = 18; i >= 0; i--) {
        if (k + (1 << i) <= N && fenwick[k + (1 << i)] <= s) {
            k += (1 << i), s -= fenwick[k];
            if (s == 0) {
                return k;
            }
        }
    }
    //No solution
    return -1;
}

int main()
{
    ifstream f("aib.in"); ofstream g("aib.out");
    f >> N >> Q;
    for(int i = 1; i <= N; i++) {
        int x; f >> x; pointUpdate(i, x);
    }
    for(int i = 1; i <= Q; i++) {
        int type, a, b; f >> type;
        if(type == 0) {
            f >> a >> b; pointUpdate(a, b);
        }
        else if(type == 1) {
            f >> a >> b; g << prefixSum(b) - prefixSum(a - 1) << '\n';
        }
        else {
            f >> a; g << prefixSearch(a) << '\n';
        }
    }
    return 0;
}
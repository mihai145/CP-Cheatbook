//
// Created by Mihai145 on 2/20/2022.
//

/*
 * Test problem: https://cses.fi/problemset/task/1654/
 * O(2^LGMAX * LGMAX)
 */


#include <iostream>
using namespace std;

const int NMAX = 2e5;
const int LGMAX = 20;
const int VALMAX = (1 << LGMAX);
int n, x[NMAX + 2], fv[VALMAX + 2];

int cnt_submask[VALMAX + 2][2], cnt_supramask[VALMAX + 2][2];
void prec_submask() {
    for(int mask = 0; mask < VALMAX; mask++) {
        cnt_submask[mask][0] = fv[mask];
        if(mask & 1) {
            cnt_submask[mask][0] += fv[mask ^ 1];
        }
    }
    for(int i = 1; i < LGMAX; i++) {
        for(int mask = 0; mask < VALMAX; mask++) {
            cnt_submask[mask][i & 1] = cnt_submask[mask][(i - 1) & 1];
            if(mask & (1 << i)) {
                cnt_submask[mask][i & 1] += cnt_submask[mask ^ (1 << i)][(i - 1) & 1];
            }
        }
    }
}
void prec_supramask() {
    for(int mask = 0; mask < VALMAX; mask++) {
        cnt_supramask[mask][0] = fv[mask];
        if(!(mask & 1)) {
            cnt_supramask[mask][0] += fv[mask ^ 1];
        }
    }
    for(int i = 1; i < LGMAX; i++) {
        for(int mask = 0; mask < VALMAX; mask++) {
            cnt_supramask[mask][i & 1] = cnt_supramask[mask][(i - 1) & 1];
            if(!(mask & (1 << i))) {
                cnt_supramask[mask][i & 1] += cnt_supramask[mask ^ (1 << i)][(i - 1) & 1];
            }
        }
    }
}

int main() {
    cin >> n;
    for(int i = 1; i <= n; i++) {
        cin >> x[i];
        fv[x[i]]++;
    }
    prec_submask(); prec_supramask();
    for(int i = 1; i <= n; i++) {
        cout << cnt_submask[x[i]][(LGMAX - 1) & 1] << ' '
            << cnt_supramask[x[i]][(LGMAX - 1) & 1] << ' '
            << n - cnt_submask[VALMAX - 1 - x[i]][(LGMAX - 1) & 1] << '\n';
    }
    return 0;
}
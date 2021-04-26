//
// Created by Mihai145 on 4/26/2021.
//

/*
 * Test problem: https://infoarena.ro/problema/scmax
 * This algorithm returns the longest *strictly* increasing subsequence
 * Input:  24 12 15 15 19
 * Output: 12 15 19
 */

#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

const int NMAX = 1e5;
int N, a[NMAX + 2], k, t[NMAX + 2], prevPosition[NMAX + 2];
int BS(int val) {
    int st = 1, dr = k, sol = 0;
    while(st <= dr) {
        int mid = (st + dr) >> 1;
        if(a[t[mid]] < val) {
            sol = mid;
            st = mid + 1;
        } else {
            dr = mid - 1;
        }
    }
    return sol;
}

int main() {
    ifstream f("scmax.in"); ofstream g("scmax.out");
    f >> N;
    for(int i = 1; i <= N; i++) {
        f >> a[i];
    }
    k = 1, t[1] = 1, prevPosition[1] = 0;
    for(int i = 2; i <= N; i++) {
        int p = BS(a[i]);
        k = max(k, p + 1);
        t[p + 1] = i, prevPosition[i] = t[p];
    }
    vector <int> sol;
    int currentPosition = t[k];
    while(currentPosition) {
        sol.push_back(a[currentPosition]);
        currentPosition = prevPosition[currentPosition];
    }
    reverse(sol.begin(), sol.end());
    g << k << '\n';
    for(int it : sol) {
        g << it << ' ';
    }
    return 0;
}
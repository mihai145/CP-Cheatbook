//
// Created by Mihai145 on 4/27/2021.
//

/*
 * Test problem: https://infoarena.ro/problema/cmap
 * O(NlogN)
 */

#include <bits/stdc++.h>
using namespace std;

const int NMAX = 1e5;
pair <int, int> v[NMAX + 5], aux[NMAX + 5];

double dist(pair<int, int> A, pair<int, int> B) {
    return sqrt(1LL * (A.first - B.first) * (A.first - B.first) + 1LL * (A.second - B.second) * (A.second - B.second));
}
long long ABS(long long x) {
    if(x < 0) { return -x; }
    return x;
}

double Solve(int st, int dr) {
    if(st == dr) {
        return 1e18;
    }

    int mid = (st + dr) >> 1, pivotX = v[mid].first;
    double d1 = Solve(st, mid), d2 = Solve(mid + 1, dr);
    double d = min(d1, d2);

    int p1 = st, p2 = mid + 1;
    int k = st - 1;
    while(k < dr) {
        k++;
        if(p1 > mid) {
            aux[k] = v[p2++];
        } else if(p2 > dr) {
            aux[k] = v[p1++];
        } else {
            if(v[p1].second < v[p2].second) {
                aux[k] = v[p1++];
            } else {
                aux[k] = v[p2++];
            }
        }
    }
    for(int i = st; i <= dr; i++) {
        v[i] = aux[i];
    }

    k = st - 1;
    for(int i = st; i <= dr; i++) {
        if(1.0 * ABS(v[i].first - pivotX) <= d) {
            aux[++k] = v[i];
        }
    }
    for(int i = st; i <= k; i++) {
        for(int j = i + 1; j <= min(i + 8, k); j++) {
            d = min(d, dist(aux[i], aux[j]));
        }
    }
    return d;
}

int main() {
    ifstream f("cmap.in"); ofstream g("cmap.out");
    int N; f >> N;
    for(int i = 1; i <= N; i++) {
        f >> v[i].first >> v[i].second;
    }
    sort(v + 1, v + N + 1);
    g << fixed << setprecision(6) << Solve(1, N) << '\n';
    return 0;
}
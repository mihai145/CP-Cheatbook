//
// Created by Mihai145 on 4/27/2021.
//

/*
 * Test problem: https://infoarena.ro/problema/infasuratoare
 * O(NlogN)
 */

#include <fstream>
#include <algorithm>
#include <iomanip>
using namespace std;

const int NMAX = 120000;
int N;
pair <double, double> v[NMAX + 2];

//--------------Stack--------------//
int top;
pair <double, double> st[NMAX + 2];
//--------------Stack--------------//

bool ccw(pair <double, double> A, pair <double, double> B, pair <double, double> C) {
    return (B.second - A.second) * (C.first - A.first) < (C.second - A.second) * (B.first - A.first);
}
bool cmp(const pair <double, double> A, const pair <double, double> B) {
    return ccw(v[1], A, B);
}

int main() {
    ifstream f("infasuratoare.in"); ofstream g("infasuratoare.out");
    f >> N;
    for(int i = 1; i <= N; i++) {
        f >> v[i].first >> v[i].second;
    }
    for(int i = 2; i <= N; i++) {
        if(v[i] < v[1]) {
            swap(v[i], v[1]);
        }
    }
    sort(v + 2, v + N + 1, cmp);
    for(int i = 1; i <= N; i++) {
        while(top > 1 && !ccw(st[top - 1], st[top], v[i])) {
            top--;
        }
        st[++top] = v[i];
    }
    g << top << '\n';
    for(int i = 1; i <= top; i++) {
        g << fixed << setprecision(6) << st[i].first << ' ' << st[i].second << '\n';
    }
    return 0;
}
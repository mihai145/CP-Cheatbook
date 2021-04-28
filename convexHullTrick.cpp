//
// Created by Mihai145 on 4/28/2021.
//

/*
 * Test problem: https://atcoder.jp/contests/dp/tasks/dp_z
 * O(N^2) -> O(N) with deque (slopes are strictly decreasing, xs from queries are strictly increasing)
 */

#include <iostream>
#include <deque>
using namespace std;

const int NMAX = 2e5;
struct Line{
    long long a, b;
    long long eval(long long x) {
        return a * x + b;
    }
};
double xIntersect(Line l1, Line l2) {
    return 1.0 * (l2.b - l1.b) / (l1.a - l2.a);
}
bool shouldPopLine(Line l1, Line l2, Line l3) { //line should not be popped <=> getx(l1, l2) < getx(l2, l3)
    return (l2.b - l1.b) * (l2.a - l3.a) >= (l1.a - l2.a) * (l3.b - l2.b);
}

long long N, C, h[NMAX + 5], minCost[NMAX + 5];
deque <Line> dq;
void insert(Line l) {
    while(dq.size() > 1 && shouldPopLine(dq.end()[-2], dq.end()[-1], l)) {
        dq.pop_back();
    }
    dq.push_back(l);
}
long long queryMin(long long x) {
    while(dq.size() > 1 && xIntersect(dq[0], dq[1]) < x) {
        dq.pop_front();
    }
    return dq[0].eval(x);
}

int main() {
    cin >> N >> C;
    for(int i = 1; i <= N; i++) {
        cin >> h[i];
    }
    minCost[1] = 0;
    for(int i = 2; i <= N; i++) {
        insert({-2 * h[i - 1], minCost[i - 1] + h[i - 1] * h[i - 1]});
        minCost[i] = C + h[i] * h[i] + queryMin(h[i]);
    }
    cout << minCost[N] << '\n';
    return 0;
}

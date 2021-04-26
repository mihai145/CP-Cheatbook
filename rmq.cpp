//
// Created by Mihai145 on 4/26/2021.
//

/*
 * Test problem: https://infoarena.ro/problema/rmq
 * Build: O(NlogN)
 * Query: O(1)
 */

#include <fstream>
#include <vector>
using namespace std;

const int NMAX = 1e5;
const int LGMAX = 18;
int N, M, log2[NMAX + 2];
vector <int> rmq[LGMAX + 5];
void BuildRmq() {
    log2[1] = 0;
    for(int i = 2; i <= N; i++) {
        log2[i] = 1 + log2[i / 2];
    }
    for(int i = 1; i <= LGMAX; i++) {
        if((1 << i) > (int)rmq[0].size()) { break; }
        for(int j = 0; j < (int)rmq[0].size(); j++) {
            if (j + (1 << i) > (int)rmq[0].size()) { break; }
            else {
                rmq[i].push_back(min(rmq[i - 1][j], rmq[i - 1][j + (1 << (i - 1))]));
            }
        }
    }
}
int QueryRmq(int x, int y) {
    if(x > y) { swap(x, y); }
    int k = log2[y - x + 1];
    return min(rmq[k][x], rmq[k][y - (1 << k) + 1]);
}

int main() {
    ifstream f("rmq.in"); ofstream g("rmq.out");
    f >> N >> M;
    for(int i = 1; i <= N; i++) {
        int x; f >> x;
        rmq[0].push_back(x);
    }
    BuildRmq();
    for(int i = 1; i <= M; i++) {
        int x, y; f >> x >> y;
        g << QueryRmq(x - 1, y - 1) << '\n';
    }
    return 0;
}
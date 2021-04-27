//
// Created by Mihai145 on 4/27/2021.
//

/*
 * Test problem: https://infoarena.ro/problema/radixsort
 * O(N) with a constant c = 4
 */

#include <fstream>
using namespace std;

const int NMAX = 1e7;
const int BYTE = 8;
int N, A, B, C;
int v[NMAX + 2], aux[NMAX + 2], cnt[(1 << BYTE) + 2];

int main() {
    ifstream f("radixsort.in"); ofstream g("radixsort.out");
    f >> N >> A >> B >> C;
    v[1] = B;
    for(int i = 2; i <= N; i++) { //generating the array
        v[i] = (1LL * A * v[i - 1] + B) % C;
    }
    for(int i = 1; i <= 4; i++) {
        for(int j = 0; j < (1 << BYTE); j++) {
            cnt[j] = 0;
        }
        for(int j = 1; j <= N; j++) {
            cnt[(v[j] >> ((i - 1) * BYTE)) & ((1 << BYTE) - 1)]++;
        }
        for(int j = 0; j < (1 << BYTE); j++) {
            cnt[j + 1] += cnt[j];
        }
        for(int j = N; j >= 1; j--) {
            aux[cnt[(v[j] >> ((i - 1) * BYTE)) & ((1 << BYTE) - 1)]--] = v[j];
        }
        for(int j = 1; j <= N; j++) {
            v[j] = aux[j];
        }
    }
    for(int i = 1; i <= N; i += 10) { //outputting the sorted array
        g << v[i] << ' ';
    }
    return 0;
}
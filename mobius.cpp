//
// Created by Mihai145 on 2/16/2022.
//

/*
 * Test problem: https://www.codechef.com/problems/COPRIME3
 * Linear sieve for precomputing values of multiplicative functions in O(N)
 */


#include <iostream>
#include <vector>
using namespace std;

const int VALMAX = 1e6;
int N, fv[VALMAX + 2];

///Linear sieve
bool is_composite[VALMAX + 2]; vector<int> primes;
int mobius[VALMAX + 2];
void prec_mobius() {
    mobius[1] = 1;
    for (int i = 2; i <= VALMAX; i++) {
        if (!is_composite[i]) { primes.push_back(i), mobius[i] = -1; }
        for (int j = 0; j < (int) primes.size() && i * primes[j] <= VALMAX; j++) {
            is_composite[i * primes[j]] = true;
            if (i % primes[j] == 0) {
                mobius[i * primes[j]] = 0;
                break;
            } else {
                mobius[i * primes[j]] = mobius[i] * mobius[primes[j]];
            }
        }
    }
}

int h[VALMAX + 2];
void prec_h() {
    for (int i = 1; i <= VALMAX; i++) {
        for (int j = i; j <= VALMAX; j += i) {
            h[i] += fv[j];
        }
    }
}

int main() {
    cin >> N;
    for(int i = 1; i <= N; i++) {
        int x; cin >> x;
        fv[x]++;
    }
    prec_mobius();
    prec_h();

    long long s2 = 0, s3 = 0;
    for(int d = 1; d <= VALMAX; d++) {
        s2 += 1LL * mobius[d] * h[d] * h[d];
        s3 += 1LL * mobius[d] * h[d] * h[d] * h[d];
    }
    cout << (s3 - fv[1] - 3 * (s2 - fv[1])) / 6 << '\n';
    return 0;
}
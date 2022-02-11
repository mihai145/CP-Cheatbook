//
// Created by Mihai145 on 2/11/2022.
//

/*
 * Test problem: https://codeforces.com/contest/1096/problem/G
 * O(NlogN) polynomial multiplication
 * O(NlogNlogK) polynomial exponentiation
 * If MOD = c*(2^k) + 1 is prime, then the (2^k)th root of unity is (G^c), where G is a primitive root of MOD
 * Corollary: If MOD is prime, G is a primitive root of MOD and n divides m-1, then the nth root of unity is G^((MOD - 1)/n)
 */

#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <vector>

using namespace std;

const int MOD = 998244353;
const int G = 3;

struct PolyMultiplier {
    unordered_map<int, int> w;

    PolyMultiplier() {
        for (int i = 1; i <= (1 << 23); i <<= 1) {
            w[i] = bin_exp(G, (MOD - 1) / i);
        }
    }

    int bin_exp(int base, int exp) {
        if (exp == 1) {
            return base;
        }

        int r = bin_exp(base, exp >> 1);
        r = 1LL * r * r % MOD;
        if (exp & 1) {
            r = 1LL * r * base % MOD;
        }
        return r;
    }

    int get_inverse(int x) {
        return bin_exp(x, MOD - 2);
    }

    static void align(vector<int> &a, vector<int> &b) {
        int n = (int) a.size() + (int) b.size() - 1;
        while ((int) a.size() < n) {
            a.push_back(0);
        }
        while ((int) b.size() < n) {
            b.push_back(0);
        }
    }

    template<typename T>
    void fft(T *in, int *out, int n, int step) {
        if (n == 1) {
            *out = *in;
            return;
        }

        int unit = w[n];
        n >>= 1;
        fft(in, out, n, step << 1);
        fft(in + step, out + n, n, step << 1);

        int t = 1;
        for (int i = 0; i < n; i++) {
            int aux = (1LL * t * out[i + n]) % MOD;
            out[i + n] = (1LL * out[i] - aux + MOD) % MOD;
            out[i] = (out[i] + aux) % MOD;
            t = (1LL * t * unit) % MOD;
        }
    }

    vector<int> evaluate(vector<int> v) {
        while (__builtin_popcount((int) v.size()) != 1) {
            v.push_back(0);
        }
        vector<int> res((int) v.size());
        fft(v.data(), res.data(), (int) v.size(), 1);
        return res;
    }

    vector<int> interpolate(vector<int> v) {
        int n = (int) v.size();
        vector<int> inv(n);
        fft(v.data(), inv.data(), n, 1);

        vector<int> res(n);
        int invN = get_inverse(n);
        for (int i = 0; i < n; i++) {
            res[i] = (1LL * inv[i] * invN) % MOD;
        }
        reverse(res.begin() + 1, res.end());
        while (!res.empty() && res.back() == 0) {
            res.pop_back();
        }
        return res;
    }

public:
    vector<int> multiply(vector<int> a, vector<int> b) {
        align(a, b);
        vector<int> A = evaluate(a), B = evaluate(b);
        vector<int> C((int) A.size());
        for (int i = 0; i < (int) A.size(); i++) {
            C[i] = (1LL * A[i] * B[i]) % MOD;
        }
        return interpolate(C);
    }
} multiplier;

void bin_pow(vector<int> &res, vector<int> &p, int exp) {
    if (exp == 1) {
        res = p;
        return;
    }

    vector<int> aux;
    bin_pow(aux, p, exp >> 1);
    res = multiplier.multiply(aux, aux);
    if (exp & 1) {
        res = multiplier.multiply(res, p);
    }
}

int main() {
    int N, K;
    cin >> N >> K;
    vector<int> poly(10, 0);
    for (int i = 1; i <= K; i++) {
        int d;
        cin >> d;
        poly[d] = 1;
    }

    vector<int> res;
    bin_pow(res, poly, N / 2);
    int ans = 0;
    for (int i = 0; i < (int) res.size(); i++) {
        ans = (ans + 1LL * res[i] * res[i] % MOD) % MOD;
    }
    cout << ans << '\n';

    return 0;
}
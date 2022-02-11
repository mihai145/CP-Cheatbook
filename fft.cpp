//
// Created by Mihai145 on 2/9/2022.
//

/*
 * Test problem: https://codeforces.com/contest/632/problem/E
 * O(NlogN) polynomial multiplication
 * O(NlogNlogK) polynomial exponentiation
 */

#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#include <iostream>
#include <algorithm>
#include <complex>
#include <vector>
using namespace std;

struct PolyMultiplier {
    typedef complex<double> ftype;
    const int NMAX = (1 << 21);
    const double PI = acos(-1);
    vector<ftype> w;

    PolyMultiplier() {
        w.resize(NMAX);
        double t = 2 * PI / NMAX;
        for (int i = 0; i < NMAX; i++) {
            w[i] = polar(1., t * i);
        }
    }

    template<typename T>
    static void align(vector<T> &a, vector<T> &b) {
        int n = (int) a.size() + (int) b.size() - 1;
        while ((int) a.size() < n) {
            a.push_back(0);
        }
        while ((int) b.size() < n) {
            b.push_back(0);
        }
    }

    template<typename T>
    void fft(T *in, ftype *out, int n, int step) {
        if (n == 1) {
            *out = *in;
            return;
        }

        int wn_step = NMAX / n;
        n >>= 1;
        fft(in, out, n, step << 1);
        fft(in + step, out + n, n, step << 1);

        for (int i = 0, j = 0; i < n; i++, j += wn_step) {
            ftype aux = w[j] * out[i + n];
            out[i + n] = out[i] - aux;
            out[i] = out[i] + aux;
        }
    }

    template<typename T>
    vector<T> interpolate(vector<ftype> v) {
        int n = (int) v.size();
        vector<ftype> inv(n);
        fft(v.data(), inv.data(), n, 1);

        vector<T> res(n);
        for (int i = 0; i < n; i++) {
            res[i] = round(real(inv[i]) / n);
        }
        reverse(res.begin() + 1, res.end());
        while (!res.empty() && res.back() == 0) {
            res.pop_back();
        }
        return res;
    }

    template<typename T>
    vector<ftype> evaluate(vector<T> v) {
        while (__builtin_popcount((int) v.size()) != 1) {
            v.push_back(0);
        }
        vector<ftype> res((int) v.size());
        fft(v.data(), res.data(), (int) v.size(), 1);
        return res;
    }

public:
    template<typename T>
    vector<T> multiply(vector<T> a, vector<T> b) {
        align(a, b);
        vector<ftype> A = evaluate(a), B = evaluate(b);
        vector<ftype> C((int) A.size());
        for (int i = 0; i < (int) A.size(); i++) {
            C[i] = A[i] * B[i];
        }
        return interpolate<T>(C);
    }
} multiplier;

void bin_pow(vector<int> &res, vector<int> &p, int exp) {
    if(exp == 1) {
        res = p;
        return;
    }

    vector<int> aux;
    bin_pow(aux, p, exp >> 1);
    res = multiplier.multiply(aux, aux);
    if(exp & 1) {
        res = multiplier.multiply(res, p);
    }

    for(int i = 0; i < (int)res.size(); i++) {
        if(res[i] != 0) {
            res[i] = 1;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, K;
    cin >> N >> K;

    vector<int> poly(1001);
    for(int i = 1; i <= N; i++) {
        int x; cin >> x;
        poly[x] = 1;
    }

    vector<int> res;
    bin_pow(res, poly, K);
    for(int i = 0; i < (int)res.size(); i++) {
        if(res[i] > 0) {
            cout << i << ' ';
        }
    }
    return 0;
}
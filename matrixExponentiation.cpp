//
// Created by Mihai145 on 1/23/2022.
//

/*
 * Test problem: https://www.pbinfo.ro/probleme/4032/zar1
 * Complexity: O(N^3*logK)
 */

#include <iostream>
#include <vector>
using namespace std;

const int NMAX = 7;
const int MOD = 1e9 + 7;
struct Matrix {
    int n, m, a[NMAX][NMAX];
    Matrix(int nn, int mm, vector <int> vals) {
        for(int i = 0; i < 7; i++) {
            for(int j = 0; j < 7; j++) {
                a[i][j] = 0;
            }
        }
        n = nn, m = mm;
        int k = 0;
        for(int i = 1; i <= nn; i++) {
            for(int j = 1; j <= mm; j++) {
                a[i][j] = vals[k++];
            }
        }
    }
    Matrix(int nn, int mm) {
        for(int i = 0; i < NMAX; i++) {
            for(int j = 0; j < NMAX; j++) {
                a[i][j] = 0;
            }
        }
        n = nn, m = mm;
    }
    Matrix operator * (const Matrix other) const {
        Matrix ans(n, other.m);
        for(int i = 1; i <= n; i++) {
            for(int j = 1; j <= other.m; j++) {
                long long f = 0LL;
                for(int k = 1; k <= m; k++)
                    f += 1LL * a[i][k] * other.a[k][j];
                ans.a[i][j] = f % MOD;
            }
        }
        return ans;
    }
};

Matrix lgPut(Matrix base, long long exp) {
    vector <int> I;
    for(int i = 1; i <= 6; i++) {
        for(int j = 1; j <= 6; j++) {
            if(i == j) {
                I.push_back(1);
            } else {
                I.push_back(0);
            }
        }
    }
    Matrix ans(6, 6, I), aux = base;
    for(long long i = 1LL; i <= exp; i <<= 1) {
        if(i & exp) {
            ans = ans * aux;
        }
        aux = aux * aux;
    }
    return ans;
}

int main() {
    long long N;
    cin >> N;
    if(N <= 6) {
        cout << (1 << (N - 1)) << '\n';
        return 0;
    }

    vector <int> a;
    for(int i = 1; i <= 6; i++) {
        a.push_back((1 << (i - 1)));
    }
    Matrix A(1, 6, a);

    vector <int> z;
    for(int i = 1; i <= 6; i++) {
        for(int j = 1; j <= 6; j++) {
            if(j != 6) {
                if(i == j + 1) {
                    z.push_back(1);
                } else {
                    z.push_back(0);
                }
            } else {
                z.push_back(1);
            }
        }
    }
    Matrix Z(6, 6, z);
    
    Z = lgPut(Z, N - 6);
    A = A * Z;
    cout << A.a[1][6] << '\n';
    return 0;
}
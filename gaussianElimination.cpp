//
// Created by Mihai145 on 4/27/2021.
//

/*
 * Test problem: https://infoarena.ro/problema/gauss
 * Gaussian elimination to transform the system into an upper triangular matrix: O(N^3)
 * Finds a solution for the system (or determines there is no solution)
 */

#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;

const int NMAX = 300;
const double EPS = 1e-12;
bool NotNull(double x) { return x < -EPS || x > EPS; }

vector<double> solveGauss(int N, int M, vector<vector<double>> sys) {
    vector<double> res(M + 1, 0);
    int line = 1, col = 1;
    while(line <= N && col <= M) {
        int k = -1;
        for(int i = line; i <= N; i++) {
            if(NotNull(sys[i][col])) { k = i; break; }
        }
        if(k == -1) { col++; continue; }

        for(int j = 1; j <= M + 1; j++) {
            swap(sys[line][j], sys[k][j]);
        }
        for(int j = col + 1; j <= M + 1; j++) {
            sys[line][j] /= sys[line][col];
        }
        sys[line][col] = 1.0;

        for(int i = line + 1; i <= N; i++) {
            for(int j = col + 1; j <= M + 1; j++) {
                sys[i][j] -= sys[line][j] * sys[i][col];
            }
            sys[i][col] = 0.0;
        }

        line++, col++;
    }
    for(int i = N; i >= 1; i--) {
        for (int j = 1; j <= M + 1; j++) {
            if (NotNull(sys[i][j])) {
                if (j == M + 1) { //the system has no solution
                    vector<double> empty;
                    return empty;
                }
                res[j] = sys[i][M + 1];
                for (int k = j + 1; k <= M; k++) {
                    res[j] -= sys[i][k] * res[k];
                }
                break;
            }
        }
    }
    return res;
}

int main() {
    ifstream f("gauss.in"); ofstream g("gauss.out");
    int N, M; f >> N >> M;
    vector<vector<double>> sys(N + 1, vector<double>(M + 2));
    for(int i = 1; i <= N; i++) {
        for(int j = 1; j <= M + 1; j++) {
            f >> sys[i][j];
        }
    }
    vector<double> res = solveGauss(N, M, sys);
    if((int)res.size() == 0) {
        g << "Imposibil\n";
    } else {
        for (int i = 1; i <= M; i++) {
            g << fixed << setprecision(10) << res[i] << ' ';
        }
    }
    return 0;
}
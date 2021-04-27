//
// Created by Mihai145 on 4/27/2021.
//

/*
 * Test problem: https://infoarena.ro/problema/strmatch
 */

#include <fstream>
#include <string>
#include <vector>
using namespace std;

const int NMAX = 2e6;
int N, M, pi[NMAX + 2];
string pattern, text;
vector <int> sol;
void Pi() {
    pi[0] = -1;
    int j = -1;
    for(int i = 1; i < N; i++) {
        while(j >= 0 && pattern[i] != pattern[j + 1]) {
            j = pi[j];
        }
        if(pattern[i] == pattern[j + 1]) {
            j++;
        }
        pi[i] = j;
    }
}
void Kmp() {
    int j = -1;
    for(int i = 0; i < M; i++) {
        while(j >= 0 && text[i] != pattern[j + 1]) {
            j = pi[j];
        }
        if(text[i] == pattern[j + 1]) {
            j++;
        }
        if(j == N - 1) {
            sol.push_back(i - N + 1);
            j = pi[j];
        }
    }
}

int main() {
    ifstream f("strmatch.in"); ofstream g("strmatch.out");
    f >> pattern >> text;
    N = (int)pattern.size(), M = (int)text.size();
    Pi(), Kmp();
    g << (int)sol.size() << '\n';
    for(int i = 0; i < min((int)sol.size(), 1000); i++) { //output the first 1000 appearances
        g << sol[i] << ' ';
    }
    return 0;
}
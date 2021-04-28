//
// Created by Mihai145 on 4/28/2021.
//

/*
 * Input:
 * accac
 *
 * Output:
 * ac LCP: 2
 * accac LCP: 0
 * c LCP: 1
 * cac LCP: 1
 * ccac LCP: 0
 */

#include <iostream>
#include <string>
using namespace std;

const int NMAX = 100000;
int N;
string s;
struct sufix {
    int startingPos;
    pair <int, int> rnk;
};
sufix v[NMAX + 2], aux[NMAX + 2];
int count[NMAX + 2], newRank[NMAX + 2], posv[NMAX + 2], lcp[NMAX + 2];
bool firstTime;

void BucketSort() {
    for(int i = 0; i <= N; i++) { count[i] = 0; }
    for(int i = 1; i <= N; i++) { count[v[i].rnk.second]++; }
    if(!firstTime) {
        firstTime = true;
        for(int i = 1; i < 26; i++) { count[i] += count[i - 1]; }
    }
    else {
        for(int i = 1; i <= N; i++) { count[i] += count[i - 1]; }
    }
    for(int i = N; i >= 1; i--) { aux[count[v[i].rnk.second]--] = v[i]; }
    for(int i = 1; i <= N; i++) { v[i] = aux[i]; }

    for(int i = 0; i <= N; i++) { count[i] = 0; }
    for(int i = 1; i <= N; i++) { count[v[i].rnk.first]++; }
    for(int i = 1; i <= N; i++) { count[i] += count[i - 1]; }
    for(int i = N; i >= 1; i--) { aux[count[v[i].rnk.first]--] = v[i]; }
    for(int i = 1; i <= N; i++) { v[i] = aux[i]; }

    int k = 0;
    newRank[1] = ++k;
    for(int i = 2; i <= N; i++) {
        if (v[i].rnk > v[i - 1].rnk) { newRank[i] = ++k; }
        else { newRank[i] = k; }
    }
    for(int i = 1; i <= N; i++) { v[i].rnk.first = newRank[i]; }
    for(int i = 1; i <= N; i++) { posv[v[i].startingPos] = i; }
}
void BuildSuffixArray() {
    for(int i = 1; i <= N; i++) {
        v[i].startingPos = i;
        v[i].rnk.first = 0;
        v[i].rnk.second = s[i - 1] - 'a';
    }
    BucketSort();
    for(int l = 2; l < 2 * N; l *= 2) {
        for(int i = 1; i <= N; i++) {
            if (v[i].startingPos + l / 2 <= N) {
                v[i].rnk.second = v[posv[v[i].startingPos + l / 2]].rnk.first;
            } else {
                v[i].rnk.second = 0;
            }
        }
        BucketSort();
    }
}
void ComputeLCP() {
    int current = 0;
    for(int i = 1; i <= N; i++) {
        int x = posv[i], posX = i, posY = v[x + 1].startingPos;
        while(posX + current <= N && posY + current <= N && s[posX + current - 1] == s[posY + current - 1]) {
            current++;
        }
        lcp[x] = current;
        current = max(0, current - 1);
    }
}

int main() {
    cin >> s; N = (int)s.size();
    BuildSuffixArray();
    ComputeLCP();
    for(int i = 1; i <= N; i++) {
        for(int j = v[i].startingPos; j <= N; j++) {
            cout << s[j - 1];
        }
        cout << " LCP: " << lcp[i] << '\n';
    }
    return 0;
}

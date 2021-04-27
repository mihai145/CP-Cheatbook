//
// Created by Mihai145 on 4/27/2021.
//

/*
 * Test problem: https://infoarena.ro/problema/pscpld
 * manacher[i] = maximum l such that s[i - l]...s[i + l] is a palindrome
 */

#include <fstream>
#include <string>
using namespace std;

const int NMAX = 1e6;
int manacher[2 * NMAX + 5];
long long ans; //count of substrings that are palindromes
void Manacher(string s) {
    int drMax = -1, posDrMax = -1;
    for(int i = 0; i < (int)s.size(); i++) {
        if(i > drMax) {
            int st = i - 1, dr = i + 1;
            while(st >= 0 && dr < (int)s.size() && s[st] == s[dr]) {
                st--, dr++;
            }
            st++, dr--;
            manacher[i] = dr - i;
            drMax = dr, posDrMax = i;

        } else {
            if(i + manacher[2 * posDrMax - i] < drMax) {
                manacher[i] = manacher[2 * posDrMax - i];
            } else {
                int st = 2 * i - drMax - 1, dr = drMax + 1;
                while(st >= 0 && dr < (int)s.size() && s[st] == s[dr]) {
                    st--, dr++;
                }
                st++, dr--;
                manacher[i] = dr - i;
                drMax = dr, posDrMax = i;
            }
        }
    }
    for(int i = 0; i < (int)s.size(); i++) {
        if(i % 2 == 0) {                        //'*' - even length palindromes
            ans += (manacher[i] + 1) / 2;
        } else {                                //'a'-'z' - odd length palindromes
            ans += (manacher[i] + 2) / 2;
        }
    }
}

int main() {
    ifstream f("pscpld.in"); ofstream g("pscpld.out");
    string s, aux; f >> aux;
    s += '*';
    for(char c : aux) {
        s += c; s += '*';
    }
    Manacher(s);
    g << ans << '\n';
    return 0;
}

//
// Created by Mihai145 on 4/25/2021.
//

/*
 * Test problem : https://infoarena.ro/problema/euclid3
 * The method returns d = gcd(a, b) and integer coefficients x and y such that a * x + b * y = d
 */

#include <fstream>
using namespace std;

int extendedGcd(int a, int b, int &x, int &y) {
    if(b == 0) {
        x = 1, y = 0;
        return a;
    }
    int x0, y0, d = extendedGcd(b, a % b, x0, y0);
    x = y0, y = x0 - (a / b) * y0;
    return d;
}

int main() {
    ifstream f("euclid3.in");
    ofstream g("euclid3.out");
    int t; f >> t;
    for(int i = 1; i <= t; i++) {
        /*
         * Task: Find integers x and y such that a * x + b * y = c
         */
        int a, b, c; f >> a >> b >> c;
        int x, y, d = extendedGcd(a, b, x, y);
        if (c % d != 0) {
            // No solution exists
            g << 0 << ' ' << 0 << '\n';
        } else {
            g << x * (c / d) << ' ' << y * (c / d) << '\n';
        }
    }
    return 0;
}
//
// Created by mihai145 on 01.12.2022.
//

#include <iostream>
#include <iomanip>
#include <complex>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

using Point = complex<double>;
using Poly = vector<Point>;
const double kPi = 4. * atan(1.);
const double kEps = 1e-11;

double cross(Point a, Point b) { return (conj(a) * b).imag(); }
Point RotatePoint(Point a, double theta) { return a * polar(1., theta); }
double det(Point a, Point b, Point c) { return cross(b - a, c - a); }

pair<Poly, Poly> ConvexHull(Poly P) {
    sort(P.begin(), P.end(), [](Point &a, Point &b) {
        return make_pair(a.real(), a.imag()) < make_pair(b.real(), b.imag());
    });
    P.erase(unique(P.begin(), P.end()), P.end());

    Poly up, dw;
    for (auto &p : P) {
        while (up.size() >= 2 && det(up[up.size() - 2], up.back(), p) >= 0) { up.pop_back(); }
        up.push_back(p);

        while (dw.size() >= 2 && det(dw[dw.size() - 2], dw.back(), p) <= 0) { dw.pop_back(); }
        dw.push_back(p);
    }

    return {up, dw};
}

struct Caliper {
    Point pivot; double angle;
    double AngleTo(Point oth) {
        double new_ang = arg(oth - pivot);
        double r = remainder(new_ang - angle, 2. * kPi);
        if (r < -kEps) { r += 2. * kPi; }
        return r;
    }
    void RotateCCW(double ang) {
        angle += ang;
        while (angle > 2. * kPi) { angle -= 2. * kPi; }
    }
    double DistanceTo(Caliper oth) {
        Point a = RotatePoint(pivot, -angle), b = RotatePoint(oth.pivot, -angle);
        return abs(a.imag() - b.imag());
    }
};

int n;
void test() {
    Poly p;
    for (int i = 0; i < n; i++) {
        int x, y; cin >> x >> y;
        p.emplace_back(1. * x, 1. * y);
    }

    pair<Poly, Poly> h = ConvexHull(p);

    Poly hull = h.second;
    for (int i = (int)h.first.size() - 2; i > 0; i--) { hull.push_back(h.first[i]); }
    hull.erase(unique(hull.begin(), hull.end()), hull.end());

    vector<Caliper> calipers(4);
    vector<int> indices(4, 0);

    for (int i = 0; i < (int)hull.size(); i++) {
        if (hull[i].imag() < hull[indices[0]].imag()) { indices[0] = i; }
        if (hull[i].real() > hull[indices[1]].real()) { indices[1] = i; }
        if (hull[i].imag() > hull[indices[2]].imag()) { indices[2] = i; }
        if (hull[i].real() < hull[indices[3]].real()) { indices[3] = i; }
    }

    for (int i = 0; i < 4; i++) {
        calipers[i].pivot = hull[indices[i]];
        calipers[i].angle = 0.5 * i * kPi;
    }

    double total = 0., min_perimeter = 1e16;
    while (total <= 2. * kPi) {
        min_perimeter = min(min_perimeter, 2 * (calipers[0].DistanceTo(calipers[2]) + calipers[1].DistanceTo(calipers[3])));

        double rot = 2. * kPi;
        int choose = -1;
        for (int i = 0; i < 4; i++) {
            double ang = calipers[i].AngleTo(hull[(indices[i] + 1) % hull.size()]);
            if (ang < rot) {
                rot = ang;
                choose = i;
            }
        }

        for (int i = 0; i < 4; i++) { calipers[i].RotateCCW(rot); }
        total += rot;
        indices[choose] = (indices[choose] + 1) % hull.size();
        calipers[choose].pivot = hull[indices[choose]];
    }

    cout << fixed << setprecision(6) << min_perimeter << '\n';
}

int main() {
    while (cin >> n) { test(); }
    return 0;
}

//
// Created by Mihai145 on 13/07/2021.
//

/*
 * Test problem: https://codeforces.com/gym/102787/problem/Z
 * Insert, Split, Merge - O(logN)
 */

#include <iostream>
#include <map>
#include <random>
#include <chrono>
#include <climits>

using namespace std;

struct Treap {
    long long priority;
    int val, sz;
    long long sum;
    Treap* leftSon;
    Treap* rightSon;
    Treap* parent;
};
Treap* emptyTreap = new Treap {
    LLONG_MIN,
    0, 0,
    0,
    NULL, NULL, NULL
};

Treap* RecomputeTree(Treap* root, Treap* leftSon, Treap* rightSon) {
    root->parent = emptyTreap;
    root->leftSon = leftSon; root->leftSon->parent = root;
    root->rightSon = rightSon; root->rightSon->parent = root;
    root->sz = root->leftSon->sz + 1 + root->rightSon->sz;
    root->sum = root->leftSon->sum + root->val + root->rightSon->sum;
    return root;
}

pair<Treap*, Treap*> Split(Treap* root, int sz) {
    if(root == emptyTreap) {
        return {emptyTreap, emptyTreap};
    }

    if(root->leftSon->sz + 1 <= sz) {
        pair<Treap*, Treap*> split = Split(root->rightSon, sz - root->leftSon->sz - 1);
        return {
            RecomputeTree(root, root->leftSon, split.first),
            split.second
        };
    }

    if(root->leftSon->sz + 1 > sz) {
        pair<Treap*, Treap*> split = Split(root->leftSon, sz);
        return {
            split.first,
            RecomputeTree(root, split.second, root->rightSon)
        };
    }
}

Treap* Merge(Treap* t1, Treap* t2) {
    if(t1 == emptyTreap) {
        return t2;
    }
    if(t2 == emptyTreap) {
        return t1;
    }

    if(t1->priority > t2->priority) {
        return RecomputeTree(t1, t1->leftSon, Merge(t1->rightSon, t2));
    }
    if(t1->priority < t2->priority) {
        return RecomputeTree(t2, Merge(t1, t2->leftSon), t2->rightSon);
    }
}

map <int, Treap*> r;
mt19937 gen(chrono::system_clock::now().time_since_epoch().count());
void Insert(int p, int val) {
    r[p] = new Treap {
        gen(),
        val, 1,
        val,
        emptyTreap,
        emptyTreap,
        emptyTreap
    };
}

Treap* FindRoot(int val) {
    Treap* f = r[val];
    while(f->parent != emptyTreap) {
        f = f->parent;
    }
    return f;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int q; cin >> q;
    for(int i = 1; i <= q; i++) {
        int t; cin >> t;
        if(t == 1) {
            int v; cin >> v;
            Insert(i, v);
        } else if(t == 2) {
            int u, v; cin >> u >> v;
            Treap* p = FindRoot(u);
            Treap* q = FindRoot(v);
            if(p != q) {
                Merge(p, q);
            }
        } else if(t == 3) {
            int y, z; cin >> y >> z;
            Treap* p = FindRoot(y);
            if(p->sz > z) {
                Split(p, z);
            }
        } else {
            int y; cin >> y;
            Treap *p = FindRoot(y);
            cout << p->sum << '\n';
        }
    }

    return 0;
}
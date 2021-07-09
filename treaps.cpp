//
// Created by Mihai145 on 09/07/2021.
//

/*
 * Test problem: https://infoarena.ro/problema/zeap
 * Insert, erase, search O(logN)
 * Find max-difference and min-difference O(logN)
 */

#include <fstream>
#include <string>
#include <random>
#include <chrono>
#include <climits>
#include <cassert>
using namespace std;
ifstream cin("zeap.in");
ofstream cout("zeap.out");

struct Treap {
    long long value;
    long long priority;
    long long minVal, maxVal, minDif;
    int sz;
    Treap* leftSon;
    Treap* rightSon;
};
Treap* emptyTreap = new Treap {
    0,
    LLONG_MIN,
    INT_MAX, INT_MIN, INT_MAX,
    0,
    NULL, NULL
};
mt19937 gen(chrono::system_clock::now().time_since_epoch().count());

Treap* RecomputeTree(Treap* root, Treap* leftSon, Treap* rightSon) {
    root->leftSon = leftSon;
    root->rightSon = rightSon;
    root->sz = root->leftSon->sz + 1 + root->rightSon->sz;
    root->minVal = min(root->value, root->leftSon->minVal);
    root->maxVal = max(root->value, root->rightSon->maxVal);
    root->minDif = min(min(root->leftSon->minDif, root->rightSon->minDif),
                       min(root->value - root->leftSon->maxVal, root->rightSon->minVal - root->value));
    return root;
}

pair<Treap*, Treap*> Split(Treap* root, int value) {
    if(root == emptyTreap) {
        return {emptyTreap, emptyTreap};
    }
    if(root->value <= value) {
        pair<Treap*, Treap*> split = Split(root->rightSon, value);
        return {
            RecomputeTree(root, root->leftSon, split.first),
            split.second
        };
    }
    if(root->value > value) {
        pair<Treap*, Treap*> split = Split(root->leftSon, value);
        return {
            split.first,
            RecomputeTree(root, split.second, root->rightSon)
        };
    }
    assert(false);
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
    } else if(t1->priority < t2->priority) {
        return RecomputeTree(t2, Merge(t1, t2->leftSon), t2->rightSon);
    }
    assert(false);
}

Treap* Insert(Treap* root, Treap* node) {
    if(root == emptyTreap) {
        return node;
    }
    if(root->priority < node->priority) {
        pair <Treap*, Treap*> split = Split(root, node-> value);
        return RecomputeTree(node, split.first, split.second);
    }
    if(node->value < root->value) {
        return RecomputeTree(root, Insert(root->leftSon, node), root->rightSon);
    }
    if(root-> value < node->value) {
        return RecomputeTree(root, root->leftSon, Insert(root->rightSon, node));
    }
    assert(false);
}

bool Find(Treap* root, int value);
Treap* Insert(Treap* root, int value) {
    Treap* newNode = new Treap {
        value,
        gen(),
        value, value, INT_MAX,
        1,
        emptyTreap, emptyTreap
    };
    if(Find(root, value) == false) {
        return Insert(root, newNode);
    } else {
        return root;
    }
}

Treap* Erase(Treap* root, int value) {
    if(root->value == value) {
        return Merge(root->leftSon, root->rightSon);
    }
    if(value < root->value) {
        return RecomputeTree(root, Erase(root->leftSon, value), root->rightSon);
    }
    if(value > root->value) {
        return RecomputeTree(root, root->leftSon, Erase(root->rightSon, value));
    }
    assert(false);
}

bool Find(Treap* root, int value) {
    if(root == emptyTreap) {
        return false;
    }
    if(root-> value == value) {
        return true;
    }
    if(value < root->value) {
        return Find(root->leftSon, value);
    }
    if(value > root->value) {
        return Find(root->rightSon, value);
    }
    assert(false);
}

int main() {
    string op; int x;
    Treap* root = emptyTreap;
    while(cin >> op) {
        if(op[0] == 'I') { ///INSERT
            cin >> x;
            root = Insert(root, x);
        } else if(op[0] == 'S') { ///ERASE
            cin >> x;
            if(Find(root, x) == false) {
                cout << -1 << '\n';
            } else {
                root = Erase(root, x);
            }
        } else if(op[0] == 'C') { ///FIND
            cin >> x;
            cout << Find(root, x) << '\n';
        } else if(op[1] == 'A') { ///MAX-DIFFERENCE
            if(root->sz <= 1) {
                cout << -1 << '\n';
            } else {
                cout << root->maxVal - root->minVal << '\n';
            }
        } else { ///MIN-DIFFERENCE
            if(root->sz <= 1) {
                cout << -1 << '\n';
            } else {
                cout << root->minDif << '\n';
            }
        }
    }

    return 0;
}

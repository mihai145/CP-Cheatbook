//
// Created by mihai145 on 03.06.2022.
//

#include <fstream>
#include <queue>

#pragma GCC optimize("O3,unroll-loops")

class InParser {
private:
    FILE *fin;
    char *buff;
    int sp;

    char read_ch() {
        ++sp;
        if (sp == 4096) {
            sp = 0;
            fread(buff, 1, 4096, fin);
        }
        return buff[sp];
    }

public:
    explicit InParser(const char *nume) {
        fin = fopen(nume, "r");
        buff = new char[4096]();
        sp = 4095;
    }

    InParser &operator>>(int &n) {
        char c;
        while (!isdigit(c = read_ch()) && c != '-');
        int sgn = 1;
        if (c == '-') {
            n = 0;
            sgn = -1;
        } else {
            n = c - '0';
        }

        while (isdigit(c = read_ch())) {
            n = 10 * n + c - '0';
        }
        n *= sgn;

        return *this;
    }
};

InParser cin("abce.in");

class OutParser {
private:
    FILE *fout;
    char *buff;
    int sp;

    void write_ch(char ch) {
        if (sp == 50000) {
            fwrite(buff, 1, 50000, fout);
            sp = 0;
            buff[sp++] = ch;
        } else {
            buff[sp++] = ch;
        }
    }

public:
    explicit OutParser(const char *name) {
        fout = fopen(name, "w");
        buff = new char[50000]();
        sp = 0;
    }

    ~OutParser() {
        fwrite(buff, 1, sp, fout);
        fclose(fout);
    }

    OutParser &operator<<(int vu32) {
        if (vu32 < 0) {
            write_ch('-');
            vu32 *= -1;
        }
        if (vu32 <= 9) {
            write_ch(vu32 + '0');
        } else {
            (*this) << (vu32 / 10);
            write_ch(vu32 % 10 + '0');
        }
        return *this;
    }

    OutParser &operator<<(char ch) {
        write_ch(ch);
        return *this;
    }

    OutParser &operator<<(const char *ch) {
        while (*ch) {
            write_ch(*ch);
            ++ch;
        }
        return *this;
    }
};

OutParser cout("abce.out");

namespace rb_color {
    enum color {
        RED = 0, BLACK = 1
    };
}

template<class>
class red_black_tree;

template<class T>
struct node {
private:
    node<T> *left_son, *right_son, *parent;
public:
    rb_color::color color;
    T val;

    template<class>
    friend
    class red_black_tree;
};

template<class T>
class red_black_tree {
private:
    node<T> *root, *nil;

    bool is_left_son(node<T> *);

    void rotate(node<T> *);

    node<T> *get_sibling(node<T> *);

    node<T> *get_uncle(node<T> *);

    node<T> *get_nephew(node<T> *);

    node<T> *get_niece(node<T> *);

    node<T> *get_min(node<T> *);

    node<T> *get_max(node<T> *);

    //void tour(node<T>*);

public:
    red_black_tree();

    void insert(T val);

    void erase(T val);

    bool find(T val);

    //void tour();
    node<T> *get_predecessor(T val);

    node<T> *get_successor(T val);

    ~red_black_tree();
};

template<class T>
red_black_tree<T>::red_black_tree() {
    nil = new node<T>;
    nil->color = rb_color::color::BLACK;
    nil->left_son = nil->right_son = nil->parent = nil;
    root = nil;
}

template<class T>
red_black_tree<T>::~red_black_tree() {
    if (root != nil) {
        std::queue<node<T> *> to_delete;
        to_delete.push(root);

        while (!to_delete.empty()) {
            node<T> *current = to_delete.front();
            to_delete.pop();
            if (current->left_son != nil) {
                to_delete.push(current->left_son);
            }
            if (current->right_son != nil) {
                to_delete.push(current->right_son);
            }
            delete current;
        }
    }

    delete nil;
}

template<class T>
bool red_black_tree<T>::is_left_son(node<T> *x) {
    if (x == x->parent->left_son) {
        return true;
    }
    return false;
}

template<class T>
void red_black_tree<T>::rotate(node<T> *x) {
    if (is_left_son(x)) {
        //right rotation
        x->parent->left_son = x->right_son;
        if (x->right_son != nil) {
            x->right_son->parent = x->parent;
        }

        node<T> *grandparent = x->parent->parent;
        if (is_left_son(x->parent)) {
            grandparent->left_son = x;
        } else {
            grandparent->right_son = x;
        }

        x->parent->parent = x;
        x->right_son = x->parent;

        x->parent = grandparent;
        if (grandparent == nil) {
            root = x;
        }
    } else {
        //left rotation
        x->parent->right_son = x->left_son;
        if (x->left_son != nil) {
            x->left_son->parent = x->parent;
        }

        node<T> *grandparent = x->parent->parent;
        if (grandparent != nil) {
            if (is_left_son(x->parent)) {
                grandparent->left_son = x;
            } else {
                grandparent->right_son = x;
            }
        }

        x->parent->parent = x;
        x->left_son = x->parent;

        x->parent = grandparent;
        if (grandparent == nil) {
            root = x;
        }
    }
}

template<class T>
node<T> *red_black_tree<T>::get_sibling(node<T> *x) {
    if (is_left_son(x)) {
        return x->parent->right_son;
    }
    return x->parent->left_son;
}

template<class T>
node<T> *red_black_tree<T>::get_uncle(node<T> *x) {
    if (is_left_son(x->parent)) {
        return x->parent->parent->right_son;
    } else {
        return x->parent->parent->left_son;
    }
}

template<class T>
node<T> *red_black_tree<T>::get_nephew(node<T> *x) {
    node<T> *sibling = get_sibling(x);
    if (is_left_son(x)) {
        return sibling->right_son;
    } else {
        return sibling->left_son;
    }
}

template<class T>
node<T> *red_black_tree<T>::get_niece(node<T> *x) {
    node<T> *sibling = get_sibling(x);
    if (is_left_son(x)) {
        return sibling->left_son;
    } else {
        return sibling->right_son;
    }
}

template<class T>
node<T> *red_black_tree<T>::get_min(node<T> *x) {
    if (x->left_son == nil) {
        return x;
    }
    return get_min(x->left_son);
}

template<class T>
node<T> *red_black_tree<T>::get_max(node<T> *x) {
    if (x->right_son == nil) {
        return x;
    }
    return get_max(x->right_son);
}

template<class T>
node<T> *red_black_tree<T>::get_predecessor(T val) { //throws 1 if strict predecessor does not exist
    node<T> *n = root, *candidate = nil;
    while (n != nil) {
        if (n->val == val) {
            if (n->left_son == nil) {
                if (candidate != nil) {
                    return candidate;
                }
                throw 1;
            } else {
                return get_max(n->left_son);
            }
        }
        if (val < n->val) {
            n = n->left_son;
        } else {
            candidate = n;
            n = n->right_son;
        }
    }
    if (candidate == nil) { throw 1; }
    return candidate;
}

template<class T>
node<T> *red_black_tree<T>::get_successor(T val) { //throws 1 if strict successor does not exist
    node<T> *n = root, *candidate = nil;
    while (n != nil) {
        if (n->val == val) {
            if (n->right_son == nil) {
                if (candidate != nil) {
                    return candidate;
                }
                throw 1;
            } else {
                return get_min(n->right_son);
            }
        }
        if (val < n->val) {
            candidate = n;
            n = n->left_son;
        } else {
            n = n->right_son;
        }
    }
    if (candidate == nil) { throw 1; }
    return candidate;
}

template<class T>
void red_black_tree<T>::insert(T val) {
    //Create a new node with the given value and color it red
    auto *x = new node<T>;
    x->val = val;
    x->color = rb_color::color::RED;
    x->parent = x->left_son = x->right_son = nil;

    if (root == nil) {
        //If the rb_tree is empty
        root = x;
    } else {
        //If the rb_tree is not empty, find the position of the new node
        auto *p = root;
        do {
            if (val < p->val) {
                if (p->left_son != nil) {
                    p = p->left_son;
                } else {
                    x->parent = p;
                    p->left_son = x;
                    break;
                }
            } else {
                if (p->right_son != nil) {
                    p = p->right_son;
                } else {
                    x->parent = p;
                    p->right_son = x;
                    break;
                }
            }
        } while (true);
    }

    //Fix insertion
    while (x != root && x->parent->color == rb_color::RED) { //trivial cases
        node<T> *uncle = get_uncle(x);
        if (uncle->color == rb_color::RED) { //uncle is red
            x->parent->color = rb_color::BLACK;
            uncle->color = rb_color::BLACK;
            x->parent->parent->color = rb_color::RED;
            x = x->parent->parent;
        } else {
            if (is_left_son(x) != is_left_son(x->parent)) { //x and his parent are LL or RR
                node<T> *old_parent = x->parent;
                rotate(x);
                x = old_parent;
            }

            x->parent->color = rb_color::BLACK;
            x->parent->parent->color = rb_color::RED;
            rotate(x->parent);
            break;
        }
    }
    root->color = rb_color::BLACK;
}

template<class T>
void red_black_tree<T>::erase(T val) {
    node<T> *x = root;
    while (x != nil) {
        if (x->val == val) {
            break;
        }
        if (val < x->val) {
            x = x->left_son;
        } else {
            x = x->right_son;
        }
    }

    if (x == nil) { //value not int rb_tree
        return;
    }

    while (x->left_son != nil || x->right_son != nil) { //x is not a leaf
        if (x->left_son != nil) {
            node<T> *predecesor = get_max(x->left_son);
            x->val = predecesor->val;
            x = predecesor;
        } else {
            node<T> *succesor = get_min(x->right_son);
            x->val = succesor->val;
            x = succesor;
        }
    }

    node<T> *to_delete = x; //keep a pointer to x, as x may move up the rb_tree

    //Fix deletion
    while (x != root && x->color != rb_color::RED) { //trivial cases
        node<T> *sibling = get_sibling(x);
        if (sibling->color == rb_color::RED) { //case I -> case II, III, IV
            x->parent->color = rb_color::RED;
            sibling->color = rb_color::BLACK;
            rotate(sibling);
        } else {
            node<T> *nephew = get_nephew(x);
            if (nephew->color == rb_color::RED) { //case II -> FINAL
                sibling->color = x->parent->color;
                x->parent->color = rb_color::BLACK;
                nephew->color = rb_color::BLACK;
                rotate(sibling);
                x = root;
                break;
            }
            node<T> *niece = get_niece(x);
            if (niece->color == rb_color::RED) { //case III -> case II -> FINAL
                niece->color = rb_color::BLACK;
                sibling->color = rb_color::RED;
                rotate(niece);
            } else { //case IV -> case I, II, III, IV
                sibling->color = rb_color::RED;
                x = x->parent;
            }
        }
    }
    x->color = rb_color::BLACK;

    //phisically deleting the node
    if (is_left_son(to_delete)) {
        to_delete->parent->left_son = nil;
    } else {
        to_delete->parent->right_son = nil;
    }
    if (to_delete == root) {
        root = nil;
    }
    delete to_delete;
}

template<class T>
bool red_black_tree<T>::find(T val) {
    if (root == nil) {
        return false;
    }

    node<T> *n = root;
    while (n != nil) {
        if (n->val == val) {
            return true;
        }
        if (val < n->val) {
            n = n->left_son;
        } else {
            n = n->right_son;
        }
    }
    return false;
}

/* PRINT ENTIRE RB_TREE
template<class T>
void red_black_tree<T>::tour(node<T>* x) {
    if(x == nil) {
        return;
    }
    tour(x->left_son);
    cout << x->val << ' ';
    tour(x->right_son);
}

template<class T>
void red_black_tree<T>::tour() { //inorder traversal
    tour(root);
    cout << '\n';
}
 */

int main() {
    red_black_tree<int> rb_tree;

    int q;
    cin >> q;
    for (int i = 1; i <= q; i++) {
        int t, x, y;
        cin >> t >> x;

        if (t == 1) {
            rb_tree.insert(x);
        } else if (t == 2) {
            rb_tree.erase(x);
        } else if (t == 3) {
            cout << rb_tree.find(x) << '\n';
        } else if (t == 4) {
            if (rb_tree.find(x)) {
                cout << x << '\n';
            } else {
                cout << rb_tree.get_predecessor(x)->val << '\n';
            }
        } else if (t == 5) {
            if (rb_tree.find(x)) {
                cout << x << '\n';
            } else {
                cout << rb_tree.get_successor(x)->val << '\n';
            }
        } else {
            cin >> y;
            int z = rb_tree.find(x) ? x : rb_tree.get_successor(x)->val;
            while (z <= y) {
                cout << z << ' ';
                try {
                    z = rb_tree.get_successor(z)->val;
                } catch (...) {
                    break;
                }
            }
            cout << '\n';
        }
    }

    return 0;
}
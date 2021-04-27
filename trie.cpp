//
// Created by Mihai145 on 4/27/2021.
//

/*
 * Test problem: https://infoarena.ro/problema/trie
 * Insert: O(wordLength)
 * Delete: O(wordLength)
 * CountAppearances: O(wordLength)
 * BestPrefix: O(wordLength)
 */

#include <fstream>
#include <string>
using namespace std;

const int SIGMA = 26;
struct Node {
    int nrap, nrc;
    Node* children[SIGMA];
    Node() {
        nrap = nrc = 0;
        for(int i = 0; i < SIGMA; i++) {
            children[i] = nullptr;
        }
    }
};
Node *trie = nullptr;
string s;

Node* InsertTrie(Node* node, int index) {
    if(node == nullptr) {
        node = new Node;
    }
    node-> nrap++;
    if(index == (int)s.size()) {
        node-> nrc++;
    } else {
        node-> children[s[index] - 'a'] = InsertTrie(node-> children[s[index] - 'a'], index + 1);
    }
    return node;
}
Node* DeleteTrie(Node* node, int index) {
    node-> nrap--;
    if(index == (int)s.size()) {
        node-> nrc--;
    } else {
        node-> children[s[index] - 'a'] = DeleteTrie(node-> children[s[index] - 'a'], index + 1);
    }
    if(node-> nrap == 0) {
        node = nullptr;
    }
    return node;
}
int Count(Node* node, int index) {
    if(node == nullptr) {
        return 0;
    }
    if(index == (int)s.size()) {
        return node-> nrc;
    }
    return Count(node-> children[s[index] - 'a'], index + 1);
}
int BestPref(Node* node, int index) {
    if(node == nullptr) {
        return -1;
    }
    if(index == (int)s.size()) {
        return 0;
    }
    return 1 + BestPref(node-> children[s[index] - 'a'], index + 1);
}

int main() {
    ifstream f("trie.in"); ofstream g("trie.out");
    int t;
    while(f >> t >> s) {
        if(t == 0) {
            trie = InsertTrie(trie, 0);
        } else if(t == 1) {
            trie = DeleteTrie(trie, 0);
        } else if(t == 2) {
            g << Count(trie, 0) << '\n';
        } else {
            g << BestPref(trie, 0) << '\n';
        }
    }
    return 0;
}
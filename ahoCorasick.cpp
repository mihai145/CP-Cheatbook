//
// Created by Mihai145 on 4/27/2021.
//

/*
 * Test problem: https://infoarena.ro/problema/ahocorasick
 * Given a string A and a dictionary with N words, find the number of occurrences of each word in the string.
 */

#include <bits/stdc++.h>
using namespace std;

const int SIGMA = 26;
struct Node {
    Node* children[SIGMA];
    Node* failLink;
    vector <int> wordList;
    int cnt;
    Node() {
        for(int i = 0; i < SIGMA; i++) {
            children[i] = nullptr;
        }
        failLink = nullptr; cnt = 0;
    }
};
Node *aho = new Node;
string text, s;
int N, freq[100 + 2];

void Insert(Node *node, int index, int wordId) {
    if(index == (int)s.size()) {
        node->wordList.push_back(wordId);
        return;
    } else {
        if(node-> children[s[index] - 'a'] == nullptr) {
            node->children[s[index] - 'a'] = new Node;
        }
        Insert(node-> children[s[index] - 'a'], index + 1, wordId);
    }
}
void ComputeAhoLinks() {
    aho-> failLink = aho;
    queue <Node*> Q;
    for(int i = 0; i < SIGMA; i++) {
        if(aho-> children[i] != nullptr) {
            aho-> children[i]-> failLink = aho;
            Q.push(aho-> children[i]);
        }
    }
    while(!Q.empty()) {
        Node* node = Q.front(); Q.pop();
        for(int i = 0; i < SIGMA; i++) {
            if(node->children[i] != nullptr) {
                Node *failNode = node-> failLink;
                while(failNode != aho && failNode-> children[i] == nullptr) {
                    failNode = failNode-> failLink;
                }
                if(failNode-> children[i] != nullptr && failNode-> children[i] != node-> children[i]) {
                    failNode = failNode-> children[i];
                }
                node->children[i]->failLink = failNode;
                Q.push(node-> children[i]);
            }
        }
    }
}
void Solve() {
    Node *node = aho;
    for(int i = 0; i < (int)text.size(); i++) {
        while(node != aho && node-> children[text[i] - 'a'] == nullptr) {
            node = node-> failLink;
        }
        if(node-> children[text[i] - 'a'] != nullptr) {
            node = node-> children[text[i] - 'a'];
        }
        node-> cnt++;
    }
    queue <Node*> Q; Q.push(aho);
    vector <Node*> antibfs;
    while(!Q.empty()) {
        Node *n = Q.front(); Q.pop();
        antibfs.push_back(n);
        for(int i = 0; i < SIGMA; i++) {
            if(n-> children[i] != nullptr) {
                Q.push(n-> children[i]);
            }
        }
    }
    reverse(antibfs.begin(), antibfs.end());
    for(Node* n : antibfs) {
        for(int wordId : n-> wordList) {
            freq[wordId] += n-> cnt;
        }
        n->failLink->cnt += n->cnt;
    }
}

int main() {
    ifstream f("ahocorasick.in"); ofstream g("ahocorasick.out");
    f >> text >> N;
    for(int i = 1; i <= N; i++) {
        f >> s; Insert(aho, 0, i);
    }
    ComputeAhoLinks(), Solve();
    for(int i = 1; i <= N; i++) {
        g << freq[i] << '\n';
    }
    return 0;
}
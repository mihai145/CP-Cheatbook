//
// Created by mihai145 on 07.06.2022.
//

#include <fstream>
#include <stack>
#include <vector>

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
InParser fin("rmq.in");


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
    explicit OutParser(const char* name) {
        fout = fopen(name, "w");
        buff = new char[50000]();
        sp = 0;
    }

    ~OutParser() {
        fwrite(buff, 1, sp, fout);
        fclose(fout);
    }

    OutParser& operator << (int vu32) {
        if (vu32 <= 9) {
            write_ch(vu32 + '0');
        } else {
            (*this) << (vu32 / 10);
            write_ch(vu32 % 10 + '0');
        }

        return *this;
    }

    OutParser& operator << (char ch) {
        write_ch(ch);
        return *this;
    }
};
OutParser fout("rmq.out");

const int NMAX = 1e5;

int N, M;
std::pair<int, int> vals[NMAX + 2];
std::vector<int> g[NMAX + 2];

int level[NMAX + 2], parent[NMAX + 2];
int k_preorder, preorder[NMAX + 2], sz[NMAX + 2];
int inlabel[NMAX + 2], ascendant[NMAX + 2], head[NMAX + 2];

inline int floor_log2(const int &x) {
    return 31 - __builtin_clz(x);
}

inline int lsb(const int &x) {
    return x & -x;
}

void dfs_preorder(const int node, const int p = -1) {
    parent[node] = p;
    preorder[node] = ++k_preorder;
    sz[node] = 1;

    for (const int &son: g[node]) {
        level[son] = level[node] + 1;
        dfs_preorder(son, node);
        sz[node] += sz[son];
    }

    int i = floor_log2((preorder[node] - 1) ^ (preorder[node] + sz[node] - 1));
    inlabel[node] = ((preorder[node] + sz[node] - 1) >> i) << i;
}

void dfs_ascendant(const int& node, const int p = -1) {
    if (p != -1) {
        if (inlabel[node] == inlabel[p]) {
            ascendant[node] = ascendant[p];
        } else {
            ascendant[node] = ascendant[p] + (1 << floor_log2(lsb(inlabel[node])));
        }
    }

    if (p == -1 || inlabel[node] != inlabel[p]) {
        head[inlabel[node]] = node;
    }

    for (const int &son: g[node]) {
        dfs_ascendant(son, node);
    }
}

void schieber_vishkin(const int& root) {
    level[root] = 0;
    dfs_preorder(root);

    ascendant[root] = (1 << floor_log2(N));
    dfs_ascendant(root);
}

inline int query(const int& x, const int& y) {
    if (inlabel[x] == inlabel[y]) {
        return (level[x] <= level[y]) ? x : y;
    }

    /* Compute inlabel[z] */
    int i = std::max(
            floor_log2(inlabel[x] ^ inlabel[y]),
            std::max(
                    floor_log2(lsb(inlabel[x])),
                    floor_log2(lsb(inlabel[y]))
            )
    );

    int j = floor_log2(lsb(((ascendant[x] & ascendant[y]) >> i) << i));
    int inlabel_z = ((inlabel[x] >> (j + 1)) << (j + 1)) | (1 << j);

    /* Compute xp */
    int xp = x;
    if (inlabel[x] != inlabel_z) {
        int k = floor_log2(ascendant[x] & ((1 << j) - 1));
        xp = parent[head[((inlabel[x] >> (k + 1)) << (k + 1)) | (1 << k)]];
    }

    /* Compute yp */
    int yp = y;
    if (inlabel[y] != inlabel_z) {
        int k = floor_log2(ascendant[y] & ((1 << j) - 1));
        yp = parent[head[((inlabel[y] >> (k + 1)) << (k + 1)) | (1 << k)]];
    }

    return (level[xp] <= level[yp]) ? xp : yp;

}

int main() {
    fin >> N >> M;

    for(int i = 1; i <= N; i++) {
        fin >> vals[i].first;
        vals[i].second = i;
    }

    std::stack<int> stk;
    std::vector<int> left(N + 1, -1), right(N + 1, -1);

    for(int i = 1; i <= N; i++) {
        while(!stk.empty() && vals[stk.top()] > vals[i]) {
            stk.pop();
        }

        left[i] = stk.empty() ? -1 : stk.top();
        stk.push(i);
    }

    while(!stk.empty()) {
        stk.pop();
    }

    for(int i = N; i >= 1; i--) {
        while(!stk.empty() && vals[stk.top()] > vals[i]) {
            stk.pop();
        }

        right[i] = stk.empty() ? -1 : stk.top();
        stk.push(i);
    }

    int root = -1;
    for(int i = 1; i <= N; i++) {
        if(left[i] == -1) {
            if(right[i] != -1) {
                g[right[i]].push_back(i);
            } else {
                root = i;
            }
        } else {
            if(right[i] == -1 || vals[left[i]].first > vals[right[i]].first) {
                g[left[i]].push_back(i);
            } else {
                g[right[i]].push_back(i);
            }
        }
    }

    schieber_vishkin(root);

    for(int i = 1; i <= M; i++) {
        int x, y; fin >> x >> y;
        int z = query(x, y);
        fout << vals[z].first << '\n';
    }

    return 0;
}
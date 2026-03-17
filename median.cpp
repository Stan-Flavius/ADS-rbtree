#include "RBNode.h"
#include <iostream>
#include <sstream>
#include <string>
#include <map>
using namespace std;
struct Half 
{
    RBTree tree;
    map<int,int> cnt;
    int total = 0;

    void safeDelete(RBNode* n)
    {
        if (n == nullptr || n == RBNode::Nil) return;
        safeDelete(n->left == RBNode::Nil ? nullptr : n->left);
        safeDelete(n->right == RBNode::Nil ? nullptr : n->right);
        n->left = nullptr;
        n->right = nullptr;
        delete n;
    }

    ~Half()
    {
        safeDelete(tree.root);
        tree.root = nullptr;
    }

    void insert(int v) 
    {
        ++total;
        if (++cnt[v] == 1) 
        {
            RBNode* n = tree.createNode(v);
            tree.RBInsert(n);
        }
    }
    void erase(int v) 
    {
        if (cnt.find(v) == cnt.end() || cnt[v] == 0) return;
        --total;
        if (--cnt[v] == 0) 
        {
            RBNode* n = tree.search(tree.root, v);
            if (!tree.isNil(n))
            {
                RBNode* detached = tree.del(n);
                detached->left = nullptr;
                detached->right = nullptr;
                delete detached;
            }
            cnt.erase(v);
        }
    }
    int maxVal() { return tree.maximum(tree.root)->key; }
    int minVal() { return tree.minimum(tree.root)->key; }
    bool empty() { return tree.isNil(tree.root); }
};
struct MedianTracker 
{
    Half lo, hi;
    void moveLoToHi() { int v = lo.maxVal(); lo.erase(v); hi.insert(v); }
    void moveHiToLo() { int v = hi.minVal(); hi.erase(v); lo.insert(v); }
    void rebalance() 
    {
        while (!lo.empty() && !hi.empty() && lo.maxVal() > hi.minVal()) 
        {
            int lm = lo.maxVal(), hm = hi.minVal();
            lo.erase(lm); hi.erase(hm);
            lo.insert(hm); hi.insert(lm);
        }
        while (lo.total > hi.total + 1) moveLoToHi();
        while (hi.total > lo.total)     moveHiToLo();
    }
    void add(int x) 
    {
        if (lo.empty() || x <= lo.maxVal()) lo.insert(x);
        else                                hi.insert(x);
        rebalance();
    }
    void remove(int x) 
    {
        if (!lo.empty() && x <= lo.maxVal()) lo.erase(x);
        else                                 hi.erase(x);
        rebalance();
    }
    void median() 
    {
        if (lo.empty())
        {
            cout << "empty\n";
            return;
        }
        cout << lo.maxVal() << "\n";
    }
};
int main() {
    MedianTracker mt;
    string line;
    while (getline(cin, line)) 
    {
        if (line.empty()) continue;
        istringstream iss(line);
        string cmd;
        iss >> cmd;
        if (cmd == "ADD") 
        {
            int x; iss >> x;
            mt.add(x);
        } 
        else 
            if (cmd == "REMOVE") 
                {
                    int x; iss >> x;
                    mt.remove(x);
                } 
            else 
                if (cmd == "MEDIAN") 
                    mt.median();
    }
    return 0;
}
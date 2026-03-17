#include "RBNode.h"       
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
using namespace std;

static const int MAX_PLAYERS = 100000;
struct Leaderboard 
{
    RBTree tree;
    map<string, int>      playerScore;   
    map<string, int>      playerUID;     
    map<string, RBNode*>  playerNode;    
    map<int,    string>   uidName;       
    int nextUID = 1;

    int encode(int score, int uid) const 
    {
        return score * MAX_PLAYERS + uid;
    }

    void add(const string& name, int score) 
    {
        if (playerScore.count(name)) 
        {
            update(name, score - playerScore[name]);
            return;
        }
        int uid = nextUID++;
        playerUID[name]  = uid;
        uidName[uid]     = name;
        playerScore[name] = score;

        RBNode* node = tree.createNode(encode(score, uid));
        playerNode[name] = node;
        tree.RBInsert(node);
    }

    void update(const string& name, int delta) 
    {
        if (!playerScore.count(name)) return;  

        RBNode* old = playerNode[name];
        RBNode* detached = tree.del(old);
        delete detached;

        int newScore = playerScore[name] + delta;
        playerScore[name] = newScore;
        int uid = playerUID[name];

        RBNode* node = tree.createNode(encode(newScore, uid));
        playerNode[name] = node;
        tree.RBInsert(node);
    }

    void remove(const string& name) 
    {
        if (!playerScore.count(name)) 
            return;

        RBNode* old = playerNode[name];
        RBNode* detached = tree.del(old);
        delete detached;

        int uid = playerUID[name];
        uidName.erase(uid);
        playerUID.erase(name);
        playerScore.erase(name);
        playerNode.erase(name);
    }

    void top(int k) {
        RBNode* cur = tree.isNil(tree.root) ? RBNode::Nil : tree.maximum(tree.root);
        int printed = 0;
        while (!tree.isNil(cur) && printed < k) 
        {
            int key = cur->key;
            int uid  = key % MAX_PLAYERS;
            int score = key / MAX_PLAYERS;
            cout << uidName[uid] << " " << score << "\n";
            ++printed;
            cur = tree.predecessor(cur);
        }
    }
};

int main() {
    Leaderboard lb;
    string line;
    while (getline(cin, line)) {
        if (line.empty()) continue;
        istringstream iss(line);
        string cmd;
        iss >> cmd;

        if (cmd == "ADD") {
            string name; int score;
            iss >> name >> score;
            lb.add(name, score);
        } else if (cmd == "UPDATE") {
            string name; int delta;
            iss >> name >> delta;
            lb.update(name, delta);
        } else if (cmd == "REMOVE") {
            string name;
            iss >> name;
            lb.remove(name);
        } else if (cmd == "TOP") {
            int k;
            iss >> k;
            lb.top(k);
        }
    }
    return 0;
}

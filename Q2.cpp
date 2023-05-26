#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <algorithm>

using namespace std;
using namespace chrono;

struct TrieNode {
    vector<TrieNode*> children;
    bool endOfWord;

    TrieNode() : endOfWord(false), children(26, nullptr) {}
};

class Trie {
private:
    TrieNode* root;

public:
    Trie() : root(new TrieNode()) {}

    void insert(const string &word) {
        TrieNode* curr = root;
        for (char c : word) {
            if (curr->children[c - 'a'] == nullptr) {
                curr->children[c - 'a'] = new TrieNode();
            }
            curr = curr->children[c - 'a'];
        }
        curr->endOfWord = true;
    }

    bool search(const string &word) {
        TrieNode* curr = root;
        for (char c : word) {
            if (curr->children[c - 'a'] == nullptr) {
                return false;
            }
            curr = curr->children[c - 'a'];
        }
        return curr->endOfWord;
    }
};

class Dictionary {
private:
    Trie trie;

public:
    Dictionary(const string &filename) {
    auto start = high_resolution_clock::now();

    ifstream newfile(filename);
    if (newfile.is_open()) {
        string word;
        while (getline(newfile, word)) {
            trie.insert(word);
        }
    }
    newfile.close();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken to load dictionary: " << duration.count() << " microseconds" << endl;
}


    bool search(const string &word) {
        return trie.search(word);
    }
};

int main() {
    Dictionary dict("dictionary.txt");
    auto start = high_resolution_clock::now();

    cout << dict.search("aaaaaa") << endl;
    cout << dict.search("world") << endl;

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by main function: " << duration.count() << " microseconds" << endl;

    return 0;
}

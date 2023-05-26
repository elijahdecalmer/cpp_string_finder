#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <regex>
#include <sstream>
using namespace std;


    vector<int> computeLPSArray(const string &pattern) {
        vector<int> lps(pattern.size());
        int len = 0;
        lps[0] = 0;
        int i = 1;
        while (i < pattern.size()) {
            if (pattern[i] == pattern[len])
                            {
                len++;
                lps[i] = len;
                i++;
            }
            else 
            {
                if (len != 0)
                {
                    len = lps[len - 1];
                  
                }
                else
                {
                    lps[i] = 0;
                    i++;
                }
            }
        }
    return lps;
    }

size_t kmpSearch(const string &text, const string &pattern) {
        vector<int> lps = computeLPSArray(pattern);
        int i = 0;
        int j = 0;
        while (i < text.size()) {
            if (pattern[j] == text[i]) {
                j++;
                i++;
            }

            if (j == pattern.size()) {
                return (i - j);
                j = lps[j - 1];
            }
            else if (i < text.size() && pattern[j] != text[i]) {
                if (j != 0)
                    j = lps[j - 1];
                else
                    i = i + 1;
            }
        }
        return string::npos;
    }



int main() {
    //cout << kmpSearch("bba", "bb");
    string word = "hello";
    cout << word.substr(1, 1);
    return 0;
}
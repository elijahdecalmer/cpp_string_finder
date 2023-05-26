#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <sstream>
#include <list>
#include <thread>
#include <map>

using namespace std;
using namespace chrono;

class PatternPart {
public:
    int min;
    int max;
    string str;
    vector<char> letters;
};

class PatternPerm {
    public:
    int length;
    vector<PatternPart> patternParts;
};

class Dictionary {
private:
    vector<vector<string>> words;
    string pattern;
    vector<PatternPart> patternParts;
    


    list<string> matches;
    vector<list<string>> matches_vec;
    vector<PatternPerm> patternPerms;
    
public:
    int min_word_length;
    int max_word_length;
    
    std::__1::chrono::nanoseconds duration;
    Dictionary(const string &filename, const string &pattern) : pattern(pattern), min_word_length(0), max_word_length(0) {
        auto start = high_resolution_clock::now();

        ifstream newfile(filename);
        if (newfile.is_open()) {
            string word;
            words.resize(35);
            while (newfile >> word) {
                words[word.size()].push_back(word);
            }
        }
        newfile.close();

        auto stop = high_resolution_clock::now();
        duration = duration_cast<nanoseconds>(stop - start);
        matches_vec.resize(30);
    }

    void setWordLength(int length){
        min_word_length = length;
        max_word_length = length;
    }

    int getMatchesSize(){
        return matches.size();
    }

        void printPattern() {
        for (const auto &part : patternParts) {
            cout << "str: " << part.str << ", letters: ";
            for (char ch : part.letters) {
                cout << ch;
            }
            cout << ", min: " << part.min << ", max: " << part.max << endl;
        }
        cout << "Min word length: " << min_word_length << ", Max word length: " << max_word_length << endl;
    }

    void interpretPattern() {
        vector<string> pattern_bits;
        string current_bit;

        for(int i=0; i < pattern.size(); ++i) {
            current_bit += pattern[i];
            
            if ((pattern[i] == '}' && i < pattern.size() - 1) ||
                (isalpha(pattern[i]) && i < pattern.size() - 1 && isdigit(pattern[i+1])) ||
                (isalpha(pattern[i]) && i < pattern.size() - 1 && pattern[i+1] == '{') ||
                i == pattern.size() - 1) 
                {
                    pattern_bits.push_back(current_bit);
                    current_bit = "";
            }
        }

        for (const auto &bit : pattern_bits) {
            PatternPart part;
            size_t pos;
            part.str = ".";

            if ((pos = bit.find(":")) != string::npos) {
                part.min = stoi(bit.substr(0, pos));
                part.max = stoi(bit.substr(pos+1, bit.find_first_of("{") - pos - 1));
            } else if (isdigit(bit[0])) {
                part.min = part.max = stoi(bit.substr(0, bit.find_first_of("{")));
            } else {
                part.min = part.max = 0;
            }

            if ((pos = bit.find("{")) != string::npos) {
                size_t endPos = bit.find("}");
                part.str = "-";
                for (size_t i = pos+1; i < endPos; ++i) {
                    part.letters.push_back(bit[i]);
                }
            } else if (isalpha(bit[bit.size()-1])) {
                part.str = bit.substr(bit.find_first_not_of("0123456789:"));
            }

            patternParts.push_back(part);
        }

        if(patternParts.back().str != "."){
            PatternPart part;
            part.str = ".";
            part.min = 0;
            part.max = 0;
            patternParts.push_back(part);
        }


        for(PatternPart part : patternParts){
            min_word_length += part.min;
            max_word_length += part.max;
            if (part.str == "-"){
                min_word_length += 1;
                max_word_length += 1;
            } else if(part.str != ".") {
                min_word_length += part.str.size();
                max_word_length += part.str.size();
            }
        }
    }


    // void createPatternPerms(){
    //     map<int, pair<int, int>> ranges;
    //     vector<int> j_vals;
    //     int num_perms = 1;
    //     for (int j = 0; j < patternParts.size(); j++)
    //     {
    //         auto part = patternParts[j];
    //         if(part.max != part.min){
    //             ranges[j] = make_pair(part.min, part.max);
    //             // cout << j << " PAIR " << part.min << " " << part.max << endl;
    //             num_perms = num_perms * (part.max - part.min + 1);
    //             j_vals.push_back(j);
    //         }
    //     }

    //     for (int i = 0; i < num_perms; i++)
    //     {
    //         if (i != 0){
    //             for (int k = j_vals.size(); k >= 0; k--){
    //                 if (ranges[k].first != ranges[k].second){
    //                     ranges[k].first += 1;
    //                     break;
    //                 }else{
    //                     ranges[k].first = patternParts[k].min;
    //                 }
                    
    //             }
    //         }


    //         PatternPerm pat_p;
    //         pat_p.length = 0;
    //         for (int j = 0; j < patternParts.size(); j++)
    //         {
    //             auto part = patternParts[j];
    //             if(part.max != part.min){
    //                 part.max = ranges[j].first;
    //                 part.min = part.max;
    //                 pat_p.patternParts.push_back(part);
    //             }else{
    //                 pat_p.patternParts.push_back(part);
    //             }
    //         }

    //         for(PatternPart part : pat_p.patternParts){
    //             pat_p.length += part.min;
    //             if (part.str == "-"){
    //                 pat_p.length += 1;
    //             } else if(part.str != ".") {
    //                 pat_p.length += part.str.size();
    //             }
    //         }
            
    //         patternPerms.push_back(pat_p);
            
            
    //     }

    //     for (auto i : patternPerms){
    //         cout << "LENGTH: " << i.length << " patterns: " << endl;
    //         for(auto j : i.patternParts){
    //             cout << "str: " << j.str << " letters: ";
    //             for(auto k : j.letters){
    //                 cout << k;
    //             }
    //             cout << " min: " << j.min << " max: " << j.max << endl;
    //         }

    //     }
        
        
    // }

    void printResults(){
        for(list<string> l : matches_vec){
            l.sort();
            matches.merge(l);
        }
        matches.sort();
        for(auto i: matches){
            cout << i << endl;
        }
    }

    void searchWithPattern() {
        for (int i = min_word_length; i <= max_word_length; i++)
        {
            for (const string& word: words[i]){
                bool valid = true;
                size_t currentIndex = 0;
                
                for (const auto& part : patternParts) {
                    if(currentIndex > 1){
                        // cout << "CURRENT INDEX: " << currentIndex << endl;
                    }
                    size_t foundIndex;
                    if (part.str == ".") {
                        if (currentIndex + part.min > i || currentIndex + part.max < i) {
                            // cout << "SDFKJLSFKJSDFKLSDFLKJ " << currentIndex << " " << part.min << " " << part.max << ": " << i << endl;
                            valid = false;
                            break;
                        } else {
                            break;
                        }
                    } else if (part.str == "-") {
                        bool charFound = false;
                        for (size_t j = currentIndex + part.min; j <= currentIndex + part.max && j < i; ++j) {
                            if (find(part.letters.begin(), part.letters.end(), word[j]) != part.letters.end()) {
                                charFound = true;
                                currentIndex = j + 1; //change index here after finding a char in {abcd}
                                break;
                            }
                        }
                        if (!charFound) {
                            valid = false;
                            break;
                        }
                    } else {
                        foundIndex = kmpSearch(word.substr(currentIndex + part.min, part.max - part.min + part.str.size()), part.str); //currentIndex + part.min, currentIndex + part.max
                        // cout << "PART STR " << part.str << "WORD SUBSTR " << word.substr(currentIndex + part.min, currentIndex + part.max + part.str.size() - 1) << " " << part.str.size() << endl;
                        if (foundIndex == string::npos) {
                            valid = false;
                            break;
                        } else {
                            currentIndex += part.min + foundIndex + part.str.size();
                        }
                    }
                }
                if (valid) {
                    cout << word << endl;
                }
            }
        }
    }

                    // if (valid) {
                    //     matches_vec[i].push_back(word);
                    // }else if (true) // some flag to indicate that range is the issue
                    // {
                    //     for(PatternPerm pattern : patternPerms){
                    //         if(pattern.length == i){
                                
                    //         }
                    //     }
                    //     if (valid) {
                    //         //matches_vec[i].push_back(word);
                    //         //break;
                    //     }
                    // }

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
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Please provide at least two arguments: filename, pattern. Optional third argument: word length." << endl;
        return 1;
    }

    string filename = argv[1];
    string pattern = argv[2];
    int wordLength = -1;

    if (argc > 3) {
        wordLength = stoi(argv[3]);
    }

    Dictionary dict(filename, pattern);    

    dict.interpretPattern();
    dict.printPattern();

    //dict.createPatternPerms();

    if (wordLength > 0){
        // dict.setWordLength(wordLength);
    }
    // START SEARCH TIMER AFTER PATTERN HAS BEEN READ AND DICTIONARY POPULATED
    auto start = high_resolution_clock::now();
    // vector<thread> threads;
    // int thread_count = 4;
    // int vectors_per_thread = (dict.max_word_length - dict.min_word_length) / thread_count;
    // for (int i = 0; i < thread_count; ++i) {
    //     vector<int> nums;
    //     for(int j = dict.min_word_length + i; j <= dict.max_word_length; j += thread_count){
    //         nums.push_back(j);
    //     }
    //     threads.push_back(thread(&Dictionary::searchWithPattern, ref(dict), nums));
    // }
    // for (thread &t : threads) {
    //     if (t.joinable()) {
    //         t.join();
    //     }
    // }
    dict.searchWithPattern();
    
    // STOP SEARCH TIMER BEFORE MERGING LISTS AND PRINTING
    auto stop = high_resolution_clock::now();

    dict.printResults();

    dict.printPattern();

    
    


    auto duration = duration_cast<nanoseconds>(stop - start);
    auto load_time = dict.duration.count()*1e-9;

    cout << "Pattern: " << pattern << ", ";
    if (wordLength > 0){
        cout << "Required size: " << wordLength << ", ";
    }
    cout << "Words matched: " << dict.getMatchesSize() << ", ";

    cout << "Load time: " << load_time << " seconds, Search time: " << duration.count()*1e-9 << " seconds" << endl;

    return 0;
}

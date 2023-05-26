#include <iostream>
#include <vector>
#include <string>

using namespace std;

class PatternPart {
public:
    int min;
    int max;
    string str;
    vector<char> letters;

};

int main() {
    string pattern = "ab1:2{abc}d3ab";
    vector<string> pattern_bits;
    string current_bit;
    vector<PatternPart> pattern_parts;

    for(int i=0; i < pattern.size(); ++i) {
        current_bit += pattern[i];
        
        if ((pattern[i] == '}' && i < pattern.size() - 1) ||
            (isalpha(pattern[i]) && i < pattern.size() - 1 && isdigit(pattern[i+1])) ||
            i == pattern.size() - 1) {
            pattern_bits.push_back(current_bit);
            current_bit = "";
        }
    }

    for (const auto &bit : pattern_bits) {
        PatternPart part;
        size_t pos;

        // check for range
        if ((pos = bit.find(":")) != string::npos) {
            part.min = stoi(bit.substr(0, pos));
            part.max = stoi(bit.substr(pos+1, bit.find_first_of("{") - pos - 1));
        } else if (isdigit(bit[0])) {  // check for single number
            part.min = part.max = stoi(bit.substr(0, bit.find_first_of("{")));
        } else {
            part.min = part.max = 0;
        }

        // check for curly braces
        if ((pos = bit.find("{")) != string::npos) {
            size_t endPos = bit.find("}");
            part.str = "-";
            for (size_t i = pos+1; i < endPos; ++i) {
                part.letters.push_back(bit[i]);
            }
        } else if (isalpha(bit[bit.size()-1])) {  // check for single char or string
            part.str = bit.substr(bit.find_first_not_of("0123456789:"));
        }

        pattern_parts.push_back(part);
    }

    for (const auto &part : pattern_parts) {
        cout << "min: " << part.min << ", max: " << part.max << ", str: " << part.str << ", letters: ";
        for (char c : part.letters) cout << c;
        cout << "\n";
    }

    return 0;
}

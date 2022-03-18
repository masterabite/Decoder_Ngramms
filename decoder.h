#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <random>
#include <cfloat>

using Map = std::unordered_map<std::string, double>;

inline Map ReadNgrams(const std::string& filename) {
    Map ans;

    std::ifstream fin(filename);
    if (!fin.is_open()) {
        return ans;
    }

    long double sum = 0, f_temp;
    std::string str_temp;

    for (int i = 0; !fin.eof(); ++i) {
        fin >> str_temp >> f_temp;

        sum += f_temp;
        ans.insert(std::make_pair(str_temp, f_temp));
    }
    fin.close();

    for (std::pair<std::string, long double> p: ans) {
        ans[p.first] /= sum;
    }

    return ans;
}

inline std::string DecodeByKey(const std::string& text, const std::string& key) {
    std::string ans;
    for (size_t i = 0; i < text.size(); ++i) {
        if (std::isalpha(text[i])) {
            bool low = text[i] >= 'a' && text[i] <= 'z';

            int index = (low ? text[i] - 'a' : text[i] - 'A');
            ans += (low ? -'A' + 'a' : 0) + key[index];
        } else {
            ans += text[i];
        }
    }

    return ans;
}

inline long double Grate(const std::string& text, const Map& dict) {
    long double ans = 0;
    size_t len = dict.begin()->first.size();

    for (size_t i = 0; i <= text.size()-len; ++i) {
        std::string word = text.substr(i, len);

        if (dict.find(word) != dict.end()) {
            ans += logl(dict.find(word)->second);
        } else {
            ans += logl(1e-5);
        }
    }

    return ans;
}

inline std::string Swap2Char(std::string &str, char c1, char c2) {
    std::string ans;
    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == c1) {
            ans += c2;
        } else if (str[i] == c2) {
            ans += c1;
        } else {
            ans += str[i];
        }
    } return ans;
}

inline std::string Decode(const std::string& text, const Map& dict) {

    std::mt19937 gen(1729);
    std::uniform_int_distribution<int> dist(0, 25);

    std::string key = "QWERTYUIOPASDFGHJKLZXCVBNM";

    std::string clear_text; //строка только из заглавных букв
    for (size_t i = 0; i < text.size(); ++i) {
        if (std::isalpha(text[i])) {
            clear_text += std::toupper(text[i]);
        }
    }
    clear_text = DecodeByKey(clear_text, key);

    long double grate, new_grate;
    int n = 10000;

    grate = Grate(clear_text, dict);

    for (int i = 0; i < n; ++i) {
        int pos1 = dist(gen);
        int pos2 = pos1;
        while (pos1 == pos2) {
            pos2 = dist(gen);
        }

        std::string new_clear_text = Swap2Char(clear_text, key[pos1], key[pos2]);
        new_grate = Grate(new_clear_text, dict);

        if (new_grate > grate) {
            grate = new_grate;
            clear_text = new_clear_text;
            std::swap(key[pos1], key[pos2]);
        }
    }

    return DecodeByKey(text, key);
}

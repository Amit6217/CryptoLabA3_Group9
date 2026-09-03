#include"../include/analysis.h"
#include<bits/stdc++.h>
using namespace std;

void frequency_analysis(string ciphertext){
    int frequency[26]={0};
    int total=0;
    for(char c:ciphertext){
        if (c >= 'a' && c <= 'z')
            c = c - 'a' + 'A';
        if (c >= 'A' && c <= 'Z'){
            frequency[c - 'A']++;
            total++;
        }
    }
    vector<pair<char, int>> result;
    for (int i = 0; i < 26; i++){
        result.push_back({'A' + i, frequency[i]});
    }
    sort(result.begin(), result.end(),[](pair<char, int> a, pair<char, int> b){
             return a.second > b.second;
        });
    cout << "       LETTER FREQUENCY ANALYSIS\n";
    cout<<left<<setw(10)<<"Letter"<<setw(10)<<"Count"<< setw(15)<<"Percentage"<<endl;
    for (auto item : result)
    {
        double percentage = 0;

        if (total > 0)
        {
            percentage =
                (item.second * 100.0) / total;
        }

        cout << left
             << setw(10) << item.first
             << setw(10) << item.second
             << fixed
             << setprecision(2)
             << percentage << "%"
             << endl;
    }

    if (total > 0){
        cout << "\nMost frequent letters: ";
        int highest = result[0].second;
        for (auto item : result){
            if (item.second == highest)
                cout << item.first << " ";
        }

        cout << endl;
    }
    cout << "\nEnglish frequency suggestion:\n";
    cout << "Common plaintext letters: E T A O I N\n";
}

void word_frequency_analysis(string ciphertext){
    vector<string> words =extract_words(ciphertext);
    map<string, int> word_count;
    for (string word : words){
        word_count[word]++;
    }
    cout << "        WORD FREQUENCY ANALYSIS\n";
    cout << "\nONE-LETTER WORDS\n";
    bool found = false;
    for (auto item : word_count){
        if (item.first.length() == 1)
        {
            cout << item.first
                 << " -> "
                 << item.second
                 << endl;

            found = true;
        }
    }

    if (!found)
    {
        cout << "None\n";
    }

    cout << "\nPossible plaintext letters: A or I\n";

    cout << "\nTWO-LETTER WORDS\n";

    found = false;

    for (auto item : word_count)
    {
        if (item.first.length() == 2)
        {
            cout << item.first
                 << " -> "
                 << item.second
                 << endl;

            found = true;
        }
    }

    if (!found)
    {
        cout << "None\n";
    }

    cout << "\nTHREE-LETTER WORDS\n";
    found = false;
    for (auto item : word_count)
    {
        if (item.first.length() == 3)
        {
            cout << item.first
                 << " -> "
                 << item.second
                 << endl;

            found = true;
        }
    }
    if (!found)
    {
        cout << "None\n";
    }

    cout << "\nREPEATED WORDS\n";

    found = false;

    for (auto item : word_count)
    {
        if (item.second > 1)
        {
            cout << item.first
                 << " -> "
                 << item.second
                 << endl;

            found = true;
        }
    }

    if (!found)
    {
        cout << "No repeated words found.\n";
    }
}

string get_pattern(string word){
    int mapping[26];
    for (int i = 0; i < 26; i++)
    {
        mapping[i] = -1;
    }
    int next_number = 0;
    string pattern = "";
    for (int i = 0; i < word.length(); i++)
    {
        int index = word[i] - 'A';

        if (mapping[index] == -1)
        {
            mapping[index] = next_number;
            next_number++;
        }
        pattern += to_string(mapping[index]);

        if (i != word.length() - 1)
        {
            pattern += "-";
        }
    }
    return pattern;
}

void pattern_analysis(string ciphertext){
    vector<string> words =
        extract_words(ciphertext);
    map<string, int> pattern_count;
    map<string, vector<string>> pattern_words;
    for (string word : words){
        string pattern =
            get_pattern(word);

        pattern_count[pattern]++;
        pattern_words[pattern].push_back(word);
    }
    cout << "           PATTERN ANALYSIS\n";
    cout << "REPEATED PATTERNS\n";
    bool found = false;
    for (auto item : pattern_count){
        if (item.second > 1)
        {
            cout << "\nPattern: "
                 << item.first
                 << endl;

            cout << "Count: "
                 << item.second
                 << endl;

            cout << "Words: ";
            for (string word :
                 pattern_words[item.first])
            {
                cout << word << " ";
            }
            cout << endl;

            found = true;
        }
    }
    if (!found)
    {
        cout << "No repeated patterns found.\n";
    }
}
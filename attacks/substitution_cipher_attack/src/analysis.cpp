#include "../include/analysis.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <map>

using namespace std;

void frequency_analysis(string ciphertext)
{
    int frequency[26] = {0};
    int total_letters = 0;

    for (int i = 0; i < ciphertext.length(); i++)
    {
        char c = ciphertext[i];

        if (c >= 'a' && c <= 'z')
        {
            c = c - 'a' + 'A';
        }

        if (c >= 'A' && c <= 'Z')
        {
            frequency[c - 'A']++;
            total_letters++;
        }
    }

    vector<pair<char, int>> result;

    for (int i = 0; i < 26; i++)
    {
        result.push_back(
            make_pair(char('A' + i), frequency[i])
        );
    }

    sort(
        result.begin(),
        result.end(),
        [](pair<char, int> a, pair<char, int> b)
        {
            return a.second > b.second;
        }
    );

    cout << "\n========================================\n";
    cout << "       LETTER FREQUENCY ANALYSIS\n";
    cout << "========================================\n\n";

    cout << left
         << setw(10) << "Letter"
         << setw(10) << "Count"
         << setw(15) << "Percentage"
         << endl;

    cout << "----------------------------------------\n";

    for (int i = 0; i < result.size(); i++)
    {
        char letter = result[i].first;
        int count = result[i].second;

        double percentage = 0.0;

        if (total_letters > 0)
        {
            percentage =
                (double)count * 100.0 / total_letters;
        }

        cout << left
             << setw(10) << letter
             << setw(10) << count
             << fixed
             << setprecision(2)
             << percentage << "%"
             << endl;
    }

    int highest_frequency = result[0].second;

    cout << "\nMost frequent ciphertext letter(s): ";

    for (int i = 0; i < result.size(); i++)
    {
        if (result[i].second == highest_frequency)
        {
            cout << result[i].first << " ";
        }
    }

    cout << endl;

    cout << "\nTotal letters analyzed: "
         << total_letters
         << endl;
}
vector<string> extract_words(string text)
{
    vector<string> words;
    string word = "";

    for (int i = 0; i < text.length(); i++)
    {
        char c = text[i];

        if (c >= 'a' && c <= 'z')
        {
            c = c - 'a' + 'A';
        }

        if (c >= 'A' && c <= 'Z')
        {
            word += c;
        }
        else
        {
            if (!word.empty())
            {
                words.push_back(word);
                word = "";
            }
        }
    }

    if (!word.empty())
    {
        words.push_back(word);
    }

    return words;
}
void word_frequency_analysis(string ciphertext)
{
    vector<string> words =
        extract_words(ciphertext);

    map<string, int> word_count;

    for (int i = 0; i < words.size(); i++)
    {
        word_count[words[i]]++;
    }

    cout << "\n========================================\n";
    cout << "        WORD FREQUENCY ANALYSIS\n";
    cout << "========================================\n";

    cout << "\nONE-LETTER WORDS\n";
    cout << "----------------------------------------\n";

    bool found = false;

    for (auto item : word_count)
    {
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

    cout << "\nPossible plaintext guesses: A or I\n";


    cout << "\nTWO-LETTER WORDS\n";
    cout << "----------------------------------------\n";

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
    cout << "----------------------------------------\n";

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
    cout << "----------------------------------------\n";

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
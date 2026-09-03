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

string get_word_pattern(string word)
{
    string pattern = "";
    char next_symbol = 'A';
    char assigned[26];

    for (int i = 0; i < 26; i++)
    {
        assigned[i] = '\0';
    }

    for (int i = 0; i < word.length(); i++)
    {
        char c = word[i];

        if (c >= 'a' && c <= 'z')
        {
            c = c - 'a' + 'A';
        }

        if (c >= 'A' && c <= 'Z')
        {
            int index = c - 'A';

            if (assigned[index] == '\0')
            {
                assigned[index] = next_symbol;
                next_symbol++;
            }

            pattern += assigned[index];
        }
    }

    return pattern;
}

void pattern_analysis(string ciphertext)
{
    vector<string> words = extract_words(ciphertext);

    cout << "\n========================================\n";
    cout << "         PATTERN ANALYSIS\n";
    cout << "========================================\n";

    // Find words with repeated letters (doubled letters)
    cout << "\nWORDS WITH DOUBLED LETTERS\n";
    cout << "----------------------------------------\n";

    bool found = false;

    for (int i = 0; i < words.size(); i++)
    {
        string w = words[i];

        for (int j = 0; j < (int)w.length() - 1; j++)
        {
            if (w[j] == w[j + 1])
            {
                cout << w << " (position "
                     << j + 1 << "-" << j + 2 << ": "
                     << w[j] << w[j] << ")" << endl;

                found = true;
                break;
            }
        }
    }

    if (!found)
    {
        cout << "None\n";
    }

    // Show letter patterns for words grouped by pattern
    cout << "\nWORD PATTERNS (letter structure)\n";
    cout << "----------------------------------------\n";

    map<string, vector<string>> pattern_groups;

    for (int i = 0; i < words.size(); i++)
    {
        string pattern = get_word_pattern(words[i]);
        bool already_listed = false;

        for (int j = 0; j < pattern_groups[pattern].size(); j++)
        {
            if (pattern_groups[pattern][j] == words[i])
            {
                already_listed = true;
                break;
            }
        }

        if (!already_listed)
        {
            pattern_groups[pattern].push_back(words[i]);
        }
    }

    // Show patterns that have multiple different words (interesting for cryptanalysis)
    cout << "\nPatterns shared by multiple distinct words:\n";

    found = false;

    for (auto item : pattern_groups)
    {
        if (item.second.size() > 1)
        {
            cout << "  Pattern " << item.first << ": ";

            for (int i = 0; i < item.second.size(); i++)
            {
                if (i > 0)
                {
                    cout << ", ";
                }

                cout << item.second[i];
            }

            cout << endl;
            found = true;
        }
    }

    if (!found)
    {
        cout << "  None\n";
    }

    // Show all unique word patterns
    cout << "\nAll word patterns:\n";

    for (auto item : pattern_groups)
    {
        for (int i = 0; i < item.second.size(); i++)
        {
            cout << "  " << item.second[i]
                 << " -> " << item.first << endl;
        }
    }
}
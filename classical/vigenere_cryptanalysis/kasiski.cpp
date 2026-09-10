#include "kasiski.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <set>

using namespace std;

vector<string> find_repeated_patterns(string ciphertext)
{
    set<string> patterns;
    ciphertext.erase(remove_if(ciphertext.begin(), ciphertext.end(),
                               [](unsigned char ch) { return !isalpha(ch); }),
                     ciphertext.end());

    // Three letters is the usual minimum for Kasiski examination.  Looking at
    // longer sequences too reduces accidental matches without hiding short ones.
    for (int length = 3; length <= 5; ++length)
    {
        for (size_t start = 0; start + length <= ciphertext.size(); ++start)
        {
            const string pattern = ciphertext.substr(start, length);
            if (ciphertext.find(pattern, start + 1) != string::npos)
                patterns.insert(pattern);
        }
    }

    return vector<string>(patterns.begin(), patterns.end());
}

vector<int> calculate_distances(string ciphertext, string pattern)
{
    vector<int> positions;
    vector<int> distances;
    size_t position = ciphertext.find(pattern);
    while (position != string::npos)
    {
        positions.push_back(static_cast<int>(position));
        position = ciphertext.find(pattern, position + 1);
    }

    for (size_t i = 0; i < positions.size(); ++i)
        for (size_t j = i + 1; j < positions.size(); ++j)
            distances.push_back(positions[j] - positions[i]);
    return distances;
}

map<int, int> find_factors(vector<int> distances)
{
    map<int, int> factors;
    for (int distance : distances)
    {
        for (int factor = 2; factor * factor <= distance; ++factor)
        {
            if (distance % factor == 0)
            {
                ++factors[factor];
                if (factor != distance / factor)
                    ++factors[distance / factor];
            }
        }
        if (distance >= 2)
            ++factors[distance];
    }
    return factors;
}

vector<int> kasiski_analysis(string ciphertext)
{
    vector<string> patterns = find_repeated_patterns(ciphertext);

    map<int, int> factor_count;

    for (const string &pattern : patterns)
    {
        vector<int> distances = calculate_distances(ciphertext, pattern);

        map<int, int> factors = find_factors(distances);

        for (auto item : factors)
        {
            if (item.first >= 2 && item.first <= 20)
            {
                factor_count[item.first] += item.second;
            }
        }
    }

    vector<pair<int, int>> candidates;

    for (auto item : factor_count)
    {
        candidates.push_back(item);
    }

    sort(candidates.begin(), candidates.end(),
         [](const pair<int, int> &a, const pair<int, int> &b)
         {
             return a.second != b.second ? a.second > b.second : a.first < b.first;
         });

    vector<int> key_lengths;

    cout << "\nKasiski Analysis\n";
    cout << "----------------\n";

    cout << "Candidate key lengths (factor votes):\n";

    for (auto item : candidates)
    {
        cout << "Length " << item.first
             << " -> Score " << item.second << endl;

        key_lengths.push_back(item.first);
    }

    return key_lengths;
}

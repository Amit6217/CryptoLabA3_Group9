#include <bits/stdc++.h>
#include "vigenere.h"
using namespace std;

map<string, vector<int>> find_repeated_patterns(string ciphertext, int minLength)
{
    map<string, vector<int>> patterns;

    for (int length = minLength; length <= minLength + 2; length++)
    {
        for (int i = 0; i <= ciphertext.length() - length; i++)
        {
            string pattern = ciphertext.substr(i, length);

            for (int j = i + length; j <= ciphertext.length() - length; j++)
            {
                if (ciphertext.substr(j, length) == pattern)
                {
                    patterns[pattern].push_back(i);
                    patterns[pattern].push_back(j);
                }
            }
        }
    }

    for (auto &entry : patterns)
    {
        vector<int> positions = entry.second;

        sort(positions.begin(), positions.end());

        positions.erase(
            unique(positions.begin(), positions.end()),
            positions.end()
        );

        entry.second = positions;
    }

    return patterns;
}

map<string, vector<int>> calculate_distances(
    map<string, vector<int>> patterns)
{
    map<string, vector<int>> distances;

    for (auto &entry : patterns)
    {
        string pattern = entry.first;
        vector<int> positions = entry.second;

        for (int i = 0; i < positions.size(); i++)
        {
            for (int j = i + 1; j < positions.size(); j++)
            {
                int distance = positions[j] - positions[i];
                distances[pattern].push_back(distance);
            }
        }
    }

    return distances;
}

vector<int> find_factors(int distance)
{
    vector<int> factors;

    for (int i = 2; i <= distance; i++)
    {
        if (distance % i == 0)
        {
            factors.push_back(i);
        }
    }

    return factors;
}

int main()
{
    ifstream file("input/ciphertext.txt");

    string text;
    string line;

    while (getline(file, line))
    {
        text += line;
    }

    string ciphertext = clean_ciphertext(text);

    cout << "Ciphertext length: "
         << ciphertext.length() << endl;

    cout << "\nFinding repeated patterns...\n\n";

    map<string, vector<int>> patterns =
        find_repeated_patterns(ciphertext, 3);

    for (auto &entry : patterns)
    {
        cout << "Pattern: " << entry.first << endl;

        cout << "Positions: ";

        for (int position : entry.second)
        {
            cout << position << " ";
        }

        cout << endl;
    }

    cout << "\nCalculating distances...\n\n";

    map<string, vector<int>> distances =
        calculate_distances(patterns);

    for (auto &entry : distances)
    {
        cout << "Pattern: " << entry.first << endl;

        cout << "Distances: ";

        for (int distance : entry.second)
        {
            cout << distance << " ";
        }

        cout << endl;
    }

    cout << "\nFactors of distances:\n\n";

    for (auto &entry : distances)
    {
        cout << "Pattern: " << entry.first << endl;

        for (int distance : entry.second)
        {
            cout << "Distance " << distance << ": ";

            vector<int> factors = find_factors(distance);

            for (int factor : factors)
            {
                cout << factor << " ";
            }

            cout << endl;
        }
    }

    return 0;
}
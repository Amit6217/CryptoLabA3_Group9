#include "frequency.h"

#include <cctype>
#include <iomanip>
#include <iostream>
#include <limits>

using namespace std;

namespace
{
const double ENGLISH_FREQUENCIES[26] = {
    0.082, 0.015, 0.028, 0.043, 0.127, 0.022, 0.020, 0.061, 0.070,
    0.002, 0.008, 0.040, 0.024, 0.067, 0.075, 0.019, 0.001, 0.060,
    0.063, 0.091, 0.028, 0.010, 0.024, 0.002, 0.020, 0.001};
}

FrequencyTable frequency_analysis(const string &group)
{
    FrequencyTable table{};
    for (unsigned char ch : group)
        if (isalpha(ch))
            ++table[toupper(ch) - 'A'];
    return table;
}

double calculate_ic(const string &text)
{
    const FrequencyTable table = frequency_analysis(text);
    int length = 0;
    double numerator = 0.0;
    for (int count : table)
    {
        length += count;
        numerator += count * (count - 1);
    }
    return length < 2 ? 0.0 : numerator / (length * (length - 1));
}

vector<string> split_into_groups(const string &ciphertext, int key_length)
{
    vector<string> groups;
    if (key_length <= 0)
        return groups;
    groups.resize(key_length);
    int letter_index = 0;
    for (char ch : ciphertext)
        if (isalpha(static_cast<unsigned char>(ch)))
            groups[letter_index++ % key_length] += static_cast<char>(toupper(ch));
    return groups;
}

int find_shift(const string &group)
{
    const FrequencyTable observed = frequency_analysis(group);
    const int length = static_cast<int>(group.size());
    if (length == 0)
        return 0;

    double best_score = numeric_limits<double>::infinity();
    int best_shift = 0;
    for (int shift = 0; shift < 26; ++shift)
    {
        double score = 0.0;
        for (int plain = 0; plain < 26; ++plain)
        {
            const int encrypted = (plain + shift) % 26;
            const double expected = ENGLISH_FREQUENCIES[plain] * length;
            const double difference = observed[encrypted] - expected;
            score += difference * difference / expected;
        }
        if (score < best_score)
        {
            best_score = score;
            best_shift = shift;
        }
    }
    return best_shift;
}

string find_key(const vector<string> &groups)
{
    string key;
    for (const string &group : groups)
        key += static_cast<char>('A' + find_shift(group));
    return key;
}

void display_frequency_table(const FrequencyTable &table, int group_number)
{
    int total = 0;
    for (int count : table)
        total += count;

    cout << "\nGroup " << group_number << " frequency table:\n";
    for (int letter = 0; letter < 26; ++letter)
    {
        const double percent = total == 0 ? 0.0 : (100.0 * table[letter] / total);
        cout << static_cast<char>('A' + letter) << ": " << setw(2) << table[letter]
             << " (" << fixed << setprecision(1) << percent << "%)";
        if (letter % 4 == 3 || letter == 25)
            cout << '\n';
        else
            cout << "    ";
    }
    cout.unsetf(ios::floatfield);
}

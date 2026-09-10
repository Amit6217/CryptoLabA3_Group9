#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include "vigenere.h"
#include "kasiski.h"
#include "frequency.h"

using namespace std;

namespace
{
string read_ciphertext(const string &path)
{
    ifstream file(path);
    string line, text;
    while (getline(file, line))
        text += line;
    return clean_ciphertext(text);
}

double average_ic(const vector<string> &groups)
{
    if (groups.empty()) return 0.0;
    double total = 0.0;
    for (const string &group : groups) total += calculate_ic(group);
    return total / groups.size();
}
}

int main(int argc, char *argv[])
{
    const string input_path = argc > 1 ? argv[1] : "input/ciphertext.txt";
    string ciphertext = read_ciphertext(input_path);
    if (ciphertext.empty() && argc == 1)
        ciphertext = read_ciphertext("classical/vigenere_cryptanalysis/input/ciphertext.txt");
    if (ciphertext.empty())
    {
        cerr << "Unable to read ciphertext. Supply its path as the first argument.\n";
        return 1;
    }

    cout << "Vigenere Cryptanalysis: Kasiski Examination and Frequency Analysis\n";
    cout << "Ciphertext length: " << ciphertext.size() << "\n";

    vector<int> key_lengths = kasiski_analysis(ciphertext);

    // IC distinguishes meaningful candidates from coincidental Kasiski factors.
    int estimated_length = 1;
    double best_ic = -1.0;
    for (int length = 1; length <= 20; ++length)
    {
        const double ic = average_ic(split_into_groups(ciphertext, length));
        cout << "IC for length " << setw(2) << length << ": "
             << fixed << setprecision(4) << ic << '\n';
        if (ic > best_ic)
        {
            best_ic = ic;
            estimated_length = length;
        }
    }
    cout.unsetf(ios::floatfield);

    const vector<string> groups = split_into_groups(ciphertext, estimated_length);
    const string key = find_key(groups);
    const string plaintext = vigenere_decrypt(ciphertext, key);
    const bool verified = verify(ciphertext, plaintext, key);

    cout << "\nEstimated key length: " << estimated_length
         << " (average IC " << fixed << setprecision(4) << best_ic << ")\n";
    for (size_t i = 0; i < groups.size(); ++i)
        display_frequency_table(frequency_analysis(groups[i]), static_cast<int>(i + 1));

    cout << "\nRecovered key: " << key << "\n";
    cout << "Recovered plaintext:\n" << plaintext << "\n";
    cout << "Verification (re-encryption matches ciphertext): "
         << (verified ? "PASS" : "FAIL") << '\n';

    return verified ? 0 : 2;
}

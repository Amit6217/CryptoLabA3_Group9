#include <iostream>
#include <vector>
#include <algorithm>

#include "../include/cipher.h"
#include "../include/utils.h"
#include "../include/analysis.h"
#include "../include/substitution.h"

using namespace std;

// Standard English frequency order for initial guesses
const string ENGLISH_FREQ_ORDER = "ETAOINSHRDLCUMWFGYPBVKJXQZ";

void print_menu()
{
    cout << "\n========================================\n";
    cout << "       CRYPTANALYSIS MENU\n";
    cout << "========================================\n";
    cout << "  1) Show frequency analysis\n";
    cout << "  2) Show word frequency analysis\n";
    cout << "  3) Show pattern analysis\n";
    cout << "  4) Auto-propose initial substitutions\n";
    cout << "  5) Add a substitution (cipher -> plain)\n";
    cout << "  6) Remove a substitution\n";
    cout << "  7) Display partial plaintext\n";
    cout << "  8) Recover key and verify\n";
    cout << "  9) Exit\n";
    cout << "========================================\n";
    cout << "Enter choice: ";
}

// Get ciphertext frequency order (most frequent first)
string get_cipher_freq_order(string ciphertext)
{
    int frequency[26] = {0};

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
        }
    }

    // Sort by frequency descending
    vector<pair<char, int>> freq_pairs;

    for (int i = 0; i < 26; i++)
    {
        freq_pairs.push_back(
            make_pair(char('A' + i), frequency[i])
        );
    }

    sort(
        freq_pairs.begin(),
        freq_pairs.end(),
        [](pair<char, int> a, pair<char, int> b)
        {
            return a.second > b.second;
        }
    );

    string order = "";

    for (int i = 0; i < freq_pairs.size(); i++)
    {
        order += freq_pairs[i].first;
    }

    return order;
}

int main()
{
    string plaintext =
        read_file("testcases/plaintext.txt");

    if (plaintext.empty())
    {
        cout << "Error: Could not read plaintext file.\n";
        return 1;
    }

    cout << "====================================\n";
    cout << "MONOALPHABETIC SUBSTITUTION CIPHER\n";
    cout << "====================================\n";

    cout << "\nPlaintext loaded successfully.\n";

    string key;

    cout << "\nEnter a 26-letter substitution key:\n";
    cout << "Example: QWERTYUIOPASDFGHJKLZXCVBNM\n";

    cin >> key;

    if (!is_valid_key(key))
    {
        cout << "\nInvalid key!\n";
        return 1;
    }

    key = convert_to_uppercase(key);

    string ciphertext = encrypt(plaintext, key);

    cout << "\nCiphertext generated successfully.\n";

    if (!write_file("outputs/ciphertext.txt", ciphertext))
    {
        cout << "Error: Could not save ciphertext.\n";
        return 1;
    }

    cout << "Ciphertext saved to outputs/ciphertext.txt\n";

    // Read back ciphertext for analysis
    string saved_ciphertext =
        read_file("outputs/ciphertext.txt");

    if (saved_ciphertext.empty())
    {
        cout << "Error: Could not read ciphertext file.\n";
        return 1;
    }

    // Initialize mapping: all unknown
    char mapping[26];

    for (int i = 0; i < 26; i++)
    {
        mapping[i] = '?';
    }

    // Step counter for decision log
    int step = 1;

    // Decision log
    cout << "\n====================================\n";
    cout << "STARTING CRYPTANALYSIS\n";
    cout << "====================================\n";
    cout << "\nUse the menu to iteratively break the cipher.\n";

    bool running = true;

    while (running)
    {
        print_menu();

        string choice;
        cin >> choice;

        if (choice == "1")
        {
            frequency_analysis(saved_ciphertext);
        }
        else if (choice == "2")
        {
            word_frequency_analysis(saved_ciphertext);
        }
        else if (choice == "3")
        {
            pattern_analysis(saved_ciphertext);
        }
        else if (choice == "4")
        {
            // Auto-propose: map by frequency order
            string cipher_order =
                get_cipher_freq_order(saved_ciphertext);

            cout << "\n========================================\n";
            cout << "    AUTO-PROPOSED SUBSTITUTIONS\n";
            cout << "========================================\n";

            cout << "\nCipher frequency order: "
                 << cipher_order << endl;
            cout << "English frequency order: "
                 << ENGLISH_FREQ_ORDER << endl;
            cout << "\nProposed mapping (top 10 most frequent):\n";

            for (int i = 0; i < 10 && i < 26; i++)
            {
                cout << "  " << cipher_order[i]
                     << " -> " << ENGLISH_FREQ_ORDER[i]
                     << endl;
            }

            cout << "\nApply these top 10 mappings? (y/n): ";

            string confirm;
            cin >> confirm;

            if (confirm == "y" || confirm == "Y")
            {
                for (int i = 0; i < 10 && i < 26; i++)
                {
                    int idx = cipher_order[i] - 'A';

                    // Check if this plaintext letter is already assigned
                    bool conflict = false;

                    for (int j = 0; j < 26; j++)
                    {
                        if (mapping[j] == ENGLISH_FREQ_ORDER[i] && j != idx)
                        {
                            conflict = true;
                            break;
                        }
                    }

                    if (!conflict)
                    {
                        mapping[idx] = ENGLISH_FREQ_ORDER[i];

                        cout << "  Step " << step++ << ": "
                             << cipher_order[i] << " -> "
                             << ENGLISH_FREQ_ORDER[i]
                             << " (frequency-based)\n";
                    }
                    else
                    {
                        cout << "  Skipped "
                             << cipher_order[i] << " -> "
                             << ENGLISH_FREQ_ORDER[i]
                             << " (conflict)\n";
                    }
                }

                display_partial_plaintext(saved_ciphertext, mapping);
            }
        }
        else if (choice == "5")
        {
            char cipher_letter, plain_letter;

            cout << "Enter cipher letter: ";
            cin >> cipher_letter;

            if (cipher_letter >= 'a' && cipher_letter <= 'z')
            {
                cipher_letter = cipher_letter - 'a' + 'A';
            }

            cout << "Enter plain letter: ";
            cin >> plain_letter;

            if (plain_letter >= 'a' && plain_letter <= 'z')
            {
                plain_letter = plain_letter - 'a' + 'A';
            }

            if (cipher_letter < 'A' || cipher_letter > 'Z' ||
                plain_letter < 'A' || plain_letter > 'Z')
            {
                cout << "Invalid letters!\n";
            }
            else
            {
                int idx = cipher_letter - 'A';

                // Check for conflicts
                bool conflict = false;

                for (int j = 0; j < 26; j++)
                {
                    if (mapping[j] == plain_letter && j != idx)
                    {
                        cout << "Conflict: "
                             << plain_letter
                             << " is already mapped from cipher letter "
                             << char('A' + j) << endl;

                        cout << "Override? (y/n): ";

                        string override_confirm;
                        cin >> override_confirm;

                        if (override_confirm == "y" || override_confirm == "Y")
                        {
                            mapping[j] = '?';
                        }
                        else
                        {
                            conflict = true;
                        }

                        break;
                    }
                }

                if (!conflict)
                {
                    char old_mapping = mapping[idx];
                    mapping[idx] = plain_letter;

                    cout << "\nStep " << step++ << ": "
                         << cipher_letter << " -> "
                         << plain_letter;

                    if (old_mapping != '?')
                    {
                        cout << " (replaced " << old_mapping << ")";
                    }

                    cout << endl;

                    display_partial_plaintext(saved_ciphertext, mapping);
                }
            }
        }
        else if (choice == "6")
        {
            char cipher_letter;

            cout << "Enter cipher letter to remove mapping: ";
            cin >> cipher_letter;

            if (cipher_letter >= 'a' && cipher_letter <= 'z')
            {
                cipher_letter = cipher_letter - 'a' + 'A';
            }

            if (cipher_letter >= 'A' && cipher_letter <= 'Z')
            {
                int idx = cipher_letter - 'A';

                if (mapping[idx] != '?')
                {
                    cout << "Removed: " << cipher_letter
                         << " -> " << mapping[idx] << endl;

                    mapping[idx] = '?';

                    cout << "Step " << step++ << ": Removed mapping for "
                         << cipher_letter << endl;
                }
                else
                {
                    cout << cipher_letter
                         << " has no mapping to remove.\n";
                }
            }
            else
            {
                cout << "Invalid letter!\n";
            }
        }
        else if (choice == "7")
        {
            display_partial_plaintext(saved_ciphertext, mapping);
        }
        else if (choice == "8")
        {
            // Check if all 26 letters are mapped
            int mapped = 0;

            for (int i = 0; i < 26; i++)
            {
                if (mapping[i] != '?')
                {
                    mapped++;
                }
            }

            if (mapped < 26)
            {
                cout << "\nWarning: Only " << mapped
                     << "/26 letters are mapped.\n";
                cout << "Continue with verification anyway? (y/n): ";

                string confirm;
                cin >> confirm;

                if (confirm != "y" && confirm != "Y")
                {
                    continue;
                }
            }

            string recovered_key = recover_key(mapping);

            cout << "\nRecovered Key: " << recovered_key << endl;
            cout << "Original Key:  " << key << endl;

            // Verify by re-encrypting
            bool success = verify_solution(
                plaintext, saved_ciphertext, recovered_key
            );

            if (success)
            {
                // Save recovered plaintext
                string recovered_plaintext =
                    apply_substitution(saved_ciphertext, mapping);

                write_file("outputs/recovered_plaintext.txt",
                          recovered_plaintext);

                cout << "\nRecovered plaintext saved to "
                     << "outputs/recovered_plaintext.txt\n";

                // Print the decision log summary
                cout << "\n========================================\n";
                cout << "    CRYPTANALYSIS COMPLETE\n";
                cout << "========================================\n";
                cout << "Total steps taken: " << step - 1 << endl;
                cout << "Key successfully recovered!\n";
            }
        }
        else if (choice == "9")
        {
            cout << "\nExiting cryptanalysis.\n";
            running = false;
        }
        else
        {
            cout << "\nInvalid choice! Please select 1-9.\n";
        }
    }

    return 0;
}
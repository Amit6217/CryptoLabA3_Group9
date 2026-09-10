#include "../include/substitution.h"
#include "../include/cipher.h"

#include <iostream>

using namespace std;

string apply_substitution(string ciphertext, char mapping[26])
{
    string result = "";

    for (int i = 0; i < ciphertext.length(); i++)
    {
        char c = ciphertext[i];

        if (c >= 'a' && c <= 'z')
        {
            c = c - 'a' + 'A';
        }

        if (c >= 'A' && c <= 'Z')
        {
            int index = c - 'A';

            if (mapping[index] != '?')
            {
                result += mapping[index];
            }
            else
            {
                result += '?';
            }
        }
        else
        {
            result += c;
        }
    }

    return result;
}

void display_partial_plaintext(string ciphertext, char mapping[26])
{
    cout << "\n========================================\n";
    cout << "       CURRENT PARTIAL PLAINTEXT\n";
    cout << "========================================\n\n";

    string partial = apply_substitution(ciphertext, mapping);

    // Show ciphertext and partial plaintext line by line
    string cipher_line = "";
    string plain_line = "";

    for (size_t i = 0; i < ciphertext.length(); i++)
    {
        char cc = ciphertext[i];
        char pc = partial[i];

        if (cc == '\n')
        {
            cout << "Cipher: " << cipher_line << endl;
            cout << "Plain:  " << plain_line << endl;
            cout << endl;

            cipher_line = "";
            plain_line = "";
        }
        else
        {
            cipher_line += cc;
            plain_line += pc;
        }
    }

    if (!cipher_line.empty())
    {
        cout << "Cipher: " << cipher_line << endl;
        cout << "Plain:  " << plain_line << endl;
    }

    // Show current mapping
    cout << "\n\nCurrent Substitution Mapping:\n";
    cout << "Cipher:  ";

    for (int i = 0; i < 26; i++)
    {
        cout << char('A' + i) << " ";
    }

    cout << "\nPlain:   ";

    for (int i = 0; i < 26; i++)
    {
        cout << mapping[i] << " ";
    }

    cout << endl;

    // Count how many letters are mapped
    int mapped = 0;

    for (int i = 0; i < 26; i++)
    {
        if (mapping[i] != '?')
        {
            mapped++;
        }
    }

    cout << "\nLetters mapped: " << mapped << "/26\n";
}

bool verify_solution(string plaintext, string ciphertext, string key)
{
    string re_encrypted = encrypt(plaintext, key);

    // Compare ignoring case
    string ct_upper = convert_to_uppercase(ciphertext);
    string re_upper = convert_to_uppercase(re_encrypted);

    if (ct_upper == re_upper)
    {
        cout << "\n========================================\n";
        cout << "       VERIFICATION SUCCESSFUL\n";
        cout << "========================================\n";
        cout << "\nRe-encrypting the recovered plaintext with\n";
        cout << "the recovered key produces the SAME ciphertext.\n";
        cout << "\nRecovered Key: " << key << endl;

        return true;
    }
    else
    {
        cout << "\n========================================\n";
        cout << "       VERIFICATION FAILED\n";
        cout << "========================================\n";
        cout << "\nRe-encrypted text does NOT match ciphertext.\n";
        cout << "The recovered key may be incorrect.\n";

        // Show first mismatch
        for (int i = 0; i < ct_upper.length() && i < re_upper.length(); i++)
        {
            if (ct_upper[i] != re_upper[i])
            {
                cout << "\nFirst mismatch at position " << i << ":\n";
                cout << "  Expected: " << ct_upper[i] << endl;
                cout << "  Got:      " << re_upper[i] << endl;

                break;
            }
        }

        return false;
    }
}

string recover_key(char mapping[26])
{
    string key = "";

    for (int i = 0; i < 26; i++)
    {
        key += mapping[i];
    }

    return key;
}

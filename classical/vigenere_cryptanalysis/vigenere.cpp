#include "vigenere.h"
#include <cctype>

using namespace std;

string clean_ciphertext(string text)
{
    string result;

    for (char ch : text)
    {
        if (isalpha(ch))
        {
            result += toupper(ch);
        }
    }

    return result;
}

string vigenere_encrypt(string plaintext, string key)
{
    plaintext = clean_ciphertext(plaintext);
    key = clean_ciphertext(key);

    if (key.empty())
        return "";

    string ciphertext;

    for (size_t i = 0; i < plaintext.length(); i++)
    {
        int p = plaintext[i] - 'A';
        int k = key[i % key.length()] - 'A';

        int c = (p + k) % 26;

        ciphertext += char('A' + c);
    }

    return ciphertext;
}

string vigenere_decrypt(string ciphertext, string key)
{
    ciphertext = clean_ciphertext(ciphertext);
    key = clean_ciphertext(key);

    if (key.empty())
        return "";

    string plaintext;

    for (size_t i = 0; i < ciphertext.length(); i++)
    {
        int c = ciphertext[i] - 'A';
        int k = key[i % key.length()] - 'A';

        int p = (c - k + 26) % 26;

        plaintext += char('A' + p);
    }

    return plaintext;
}

bool verify(string ciphertext, string plaintext, string key)
{
    return clean_ciphertext(ciphertext) == vigenere_encrypt(plaintext, key);
}

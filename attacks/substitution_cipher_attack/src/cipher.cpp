#include "../include/cipher.h"

using namespace std;

string convert_to_uppercase(string text)
{
    for (int i = 0; i < text.length(); i++)
    {
        if (text[i] >= 'a' && text[i] <= 'z')
        {
            text[i] = text[i] - 'a' + 'A';
        }
    }

    return text;
}

bool is_valid_key(string key)
{
    if (key.length() != 26)
    {
        return false;
    }

    key = convert_to_uppercase(key);

    bool used[26] = {false};

    for (int i = 0; i < key.length(); i++)
    {
        char c = key[i];

        if (c < 'A' || c > 'Z')
        {
            return false;
        }

        int index = c - 'A';

        if (used[index])
        {
            return false;
        }

        used[index] = true;
    }

    return true;
}

string encrypt(string plaintext, string key)
{
    string ciphertext = "";

    key = convert_to_uppercase(key);

    for (int i = 0; i < plaintext.length(); i++)
    {
        char c = plaintext[i];

        if (c >= 'a' && c <= 'z')
        {
            c = c - 'a' + 'A';
        }

        if (c >= 'A' && c <= 'Z')
        {
            int index = c - 'A';

            ciphertext += key[index];
        }
        else
        {
            ciphertext += c;
        }
    }

    return ciphertext;
}

string decrypt(string ciphertext, string key)
{
    string plaintext = "";

    key = convert_to_uppercase(key);

    for (int i = 0; i < ciphertext.length(); i++)
    {
        char c = ciphertext[i];

        if (c >= 'a' && c <= 'z')
        {
            c = c - 'a' + 'A';
        }

        if (c >= 'A' && c <= 'Z')
        {
            bool found = false;

            for (int j = 0; j < 26; j++)
            {
                if (key[j] == c)
                {
                    plaintext += char('A' + j);
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                plaintext += '?';
            }
        }
        else
        {
            plaintext += c;
        }
    }

    return plaintext;
}
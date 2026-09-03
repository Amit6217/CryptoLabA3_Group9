#include <iostream>

#include "../include/cipher.h"

using namespace std;

int main()
{
    string plaintext = "HELLO WORLD";

    string key;

    cout << "====================================\n";
    cout << "MONOALPHABETIC SUBSTITUTION CIPHER\n";
    cout << "====================================\n\n";

    cout << "Plaintext: " << plaintext << endl;

    cout << "\nEnter a 26-letter substitution key:\n";
    cout << "Example: QWERTYUIOPASDFGHJKLZXCVBNM\n";

    cin >> key;

    if (!is_valid_key(key))
    {
        cout << "\nInvalid key!\n";
        cout << "The key must contain all 26 English letters exactly once.\n";

        return 1;
    }

    string ciphertext = encrypt(plaintext, key);

    cout << "\nCiphertext: " << ciphertext << endl;

    string decrypted_text = decrypt(ciphertext, key);

    cout << "Decrypted text: " << decrypted_text << endl;

    if (convert_to_uppercase(plaintext) == decrypted_text)
    {
        cout << "\nTEST PASSED: Encryption and decryption are correct.\n";
    }
    else
    {
        cout << "\nTEST FAILED.\n";
    }

    return 0;
}
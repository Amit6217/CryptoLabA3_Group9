#include <iostream>

#include "../include/cipher.h"
#include "../include/utils.h"

using namespace std;

int main()
{
    string plaintext =
        read_file("testcases/plaintext.txt");

    if (plaintext.empty())
    {
        cout << "Error: Could not read plaintext file.\n";

        cout << "Make sure the file exists at:\n";
        cout << "testcases/plaintext.txt\n";

        return 1;
    }

    cout << "====================================\n";
    cout << "MONOALPHABETIC SUBSTITUTION CIPHER\n";
    cout << "====================================\n";

    cout << "\nPlaintext loaded successfully.\n";

    cout << "\nFirst 200 characters of plaintext:\n";
    cout << "------------------------------------\n";

    cout << plaintext.substr(0, 200) << endl;

    string key;

    cout << "\nEnter a 26-letter substitution key:\n";
    cout << "Example: QWERTYUIOPASDFGHJKLZXCVBNM\n";

    cin >> key;

    if (!is_valid_key(key))
    {
        cout << "\nInvalid key!\n";
        cout << "The key must contain all 26 letters exactly once.\n";

        return 1;
    }

    string ciphertext =
        encrypt(plaintext, key);

    cout << "\nCiphertext generated successfully.\n";

    if (write_file(
            "outputs/ciphertext.txt",
            ciphertext
        ))
    {
        cout << "Ciphertext saved to:\n";
        cout << "outputs/ciphertext.txt\n";
    }
    else
    {
        cout << "Error: Could not save ciphertext.\n";

        return 1;
    }

    string saved_ciphertext =
        read_file("outputs/ciphertext.txt");

    if (saved_ciphertext.empty())
    {
        cout << "\nError: Could not read ciphertext file.\n";

        return 1;
    }

    cout << "\nCiphertext file read successfully.\n";

    cout << "\nFirst 200 characters of ciphertext:\n";
    cout << "------------------------------------\n";

    cout << saved_ciphertext.substr(0, 200) << endl;

    string decrypted_text =
        decrypt(saved_ciphertext, key);

    string original_text =
        convert_to_uppercase(plaintext);

    if (decrypted_text == original_text)
    {
        cout << "\n====================================\n";
        cout << "TEST PASSED\n";
        cout << "====================================\n";

        cout << "File reading works.\n";
        cout << "Encryption works.\n";
        cout << "Ciphertext saving works.\n";
        cout << "Ciphertext reading works.\n";
        cout << "Decryption works correctly.\n";
    }
    else
    {
        cout << "\n====================================\n";
        cout << "TEST FAILED\n";
        cout << "====================================\n";

        cout << "Decrypted text does not match plaintext.\n";
    }

    return 0;
}
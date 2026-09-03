#include <iostream>

#include "../include/cipher.h"
#include "../include/utils.h"
#include "../include/analysis.h"

using namespace std;

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

    string ciphertext =
        encrypt(plaintext, key);

    cout << "\nCiphertext generated successfully.\n";

    if (!write_file(
            "outputs/ciphertext.txt",
            ciphertext
        ))
    {
        cout << "Error: Could not save ciphertext.\n";
        return 1;
    }

    cout << "Ciphertext saved successfully.\n";

    string saved_ciphertext =
        read_file("outputs/ciphertext.txt");

    if (saved_ciphertext.empty())
    {
        cout << "Error: Could not read ciphertext file.\n";
        return 1;
    }

    cout << "\n====================================\n";
    cout << "TESTING FREQUENCY ANALYSIS\n";
    cout << "====================================\n";

    frequency_analysis(saved_ciphertext);

    cout << "\n====================================\n";
    cout << "TESTING WORD FREQUENCY ANALYSIS\n";
    cout << "====================================\n";

    frequency_analysis(saved_ciphertext);
    word_frequency_analysis(saved_ciphertext);

    return 0;
}
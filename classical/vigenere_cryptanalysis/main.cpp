#include <iostream>
#include "vigenere.h"

using namespace std;

int main()
{
    string plaintext = "ATTACKATDAWN";
    string key = "LEMON";

    string ciphertext = vigenere_encrypt(plaintext, key);

    cout << "Plaintext  : " << plaintext << endl;
    cout << "Key        : " << key << endl;
    cout << "Ciphertext : " << ciphertext << endl;

    string decrypted = vigenere_decrypt(ciphertext, key);

    cout << "Decrypted  : " << decrypted << endl;

    return 0;
}
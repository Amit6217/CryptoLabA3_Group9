#ifndef VIGENERE_H
#define VIGENERE_H

#include <string>

using namespace std;

string clean_ciphertext(string text);

string vigenere_encrypt(string plaintext, string key);

string vigenere_decrypt(string ciphertext, string key);

#endif
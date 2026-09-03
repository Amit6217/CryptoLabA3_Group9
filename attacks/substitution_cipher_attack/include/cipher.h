#ifndef CIPHER_H
#define CIPHER_H

#include <string>

using namespace std;

bool is_valid_key(string key);

string convert_to_uppercase(string text);

string encrypt(string plaintext, string key);

string decrypt(string ciphertext, string key);

#endif
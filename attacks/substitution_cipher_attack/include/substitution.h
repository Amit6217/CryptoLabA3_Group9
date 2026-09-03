#ifndef SUBSTITUTION_H
#define SUBSTITUTION_H

#include <string>

using namespace std;

// Apply a partial substitution mapping to ciphertext
// mapping[0..25]: mapping[i] = plaintext letter for cipher letter 'A'+i, or '?' if unknown
string apply_substitution(string ciphertext, char mapping[26]);

// Display the current partial plaintext with known/unknown letters
void display_partial_plaintext(string ciphertext, char mapping[26]);

// Re-encrypt plaintext with key and verify it matches the ciphertext
bool verify_solution(string plaintext, string ciphertext, string key);

// Recover the 26-letter key from the mapping array
string recover_key(char mapping[26]);

#endif

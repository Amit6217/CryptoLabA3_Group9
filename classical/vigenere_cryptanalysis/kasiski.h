#ifndef KASISKI_H
#define KASISKI_H

#include <string>
#include <vector>
#include <map>

using namespace std;

vector<string> find_repeated_patterns(string ciphertext);

vector<int> calculate_distances(string ciphertext, string pattern);

map<int, int> find_factors(vector<int> distances);

vector<int> kasiski_analysis(string ciphertext);

#endif

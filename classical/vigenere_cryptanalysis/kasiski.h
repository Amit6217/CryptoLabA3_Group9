#ifndef KASISKI_H
#define KASISKI_H

#include <string>
#include <vector>
#include <map>

using namespace std;

map<string, vector<int>> find_repeated_patterns(string ciphertext, int minLength);

map<string, vector<int>> calculate_distances(
    map<string, vector<int>> patterns
);

vector<int> find_factors(int distance);

#endif
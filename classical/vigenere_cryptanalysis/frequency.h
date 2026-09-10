#ifndef FREQUENCY_H
#define FREQUENCY_H

#include <array>
#include <string>
#include <vector>

using namespace std;

using FrequencyTable = array<int, 26>;

double calculate_ic(const string &text);
vector<string> split_into_groups(const string &ciphertext, int key_length);
FrequencyTable frequency_analysis(const string &group);
int find_shift(const string &group);
string find_key(const vector<string> &groups);
void display_frequency_table(const FrequencyTable &table, int group_number);

#endif

#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <string>
#include <vector>

using namespace std;

vector<string> extract_words(string text);

void frequency_analysis(string ciphertext);

void word_frequency_analysis(string ciphertext);

string get_pattern(string word);

void pattern_analysis(string ciphertext);

#endif
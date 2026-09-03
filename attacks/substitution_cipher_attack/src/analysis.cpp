#include"../include/analysis.h"
#include<bits/stdc++.h>
using namespace std;

void frequency_analysis(string ciphertext){
    int frequency[26]={0};
    int total=0;
    for(char c:ciphertext){
        if (c >= 'a' && c <= 'z')
            c = c - 'a' + 'A';
        if (c >= 'A' && c <= 'Z'){
            frequency[c - 'A']++;
            total++;
        }
    }
    vector<pair<char, int>> result;
    for (int i = 0; i < 26; i++){
        result.push_back({'A' + i, frequency[i]});
    }
    sort(result.begin(), result.end(),[](pair<char, int> a, pair<char, int> b){
             return a.second > b.second;
        });
    cout << "       LETTER FREQUENCY ANALYSIS\n";
    cout<<left<<setw(10)<<"Letter"<<setw(10)<<"Count"<< setw(15)<<"Percentage"<<endl;
    for (auto item : result)
    {
        double percentage = 0;

        if (total > 0)
        {
            percentage =
                (item.second * 100.0) / total;
        }

        cout << left
             << setw(10) << item.first
             << setw(10) << item.second
             << fixed
             << setprecision(2)
             << percentage << "%"
             << endl;
    }

    if (total > 0){
        cout << "\nMost frequent letters: ";
        int highest = result[0].second;
        for (auto item : result){
            if (item.second == highest)
                cout << item.first << " ";
        }

        cout << endl;
    }
    cout << "\nEnglish frequency suggestion:\n";
    cout << "Common plaintext letters: E T A O I N\n";
}


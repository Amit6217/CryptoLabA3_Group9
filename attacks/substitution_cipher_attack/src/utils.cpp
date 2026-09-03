#include "../include/utils.h"

#include <fstream>

using namespace std;

string read_file(string filename)
{
    ifstream file(filename);

    if (!file)
    {
        return "";
    }

    string content = "";
    string line;

    while (getline(file, line))
    {
        content += line;
        content += '\n';
    }

    file.close();

    return content;
}

bool write_file(string filename, string data)
{
    ofstream file(filename);

    if (!file)
    {
        return false;
    }

    file << data;

    file.close();

    return true;
}
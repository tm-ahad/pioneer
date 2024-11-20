#include "split.h"

using namespace std;

vector<string> split(const string& str, const string& delimiter) 
{
    vector<string> tokens;
    size_t start = 0;
    size_t end;

    while ((end = str.find(delimiter, start)) != string::npos) 
    {
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
    }

    tokens.push_back(str.substr(start));
    return tokens;
}
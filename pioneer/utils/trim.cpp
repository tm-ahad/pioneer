#include "trim.h"
#include "../book.h"

using namespace std;

string trim(const string& str) 
{
    auto start = find_if_not(str.begin(), str.end(), [](unsigned char ch) 
    {
        return isspace(ch) || ch == '\n' || ch == '\r';
    });

    auto end = find_if_not(str.rbegin(), str.rend(), [](unsigned char ch) 
    {
        return isspace(ch) || ch == '\n' || ch == '\r';
    }).base();

    return (start < end) ? string(start, end) : string();
}

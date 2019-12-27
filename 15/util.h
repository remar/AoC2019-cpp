#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

using namespace std;

vector<string> read_file(string path);
vector<string> split_string_to_strings(string input);
vector<int> split_string_to_ints(string input);
vector<long long int> split_string_to_long_longs(string input);
string& ltrim(string& str, const string& chars = "\t\n\v\f\r ");
string& rtrim(string& str, const string& chars = "\t\n\v\f\r ");
string& trim(string& str, const string& chars = "\t\n\v\f\r ");

#endif

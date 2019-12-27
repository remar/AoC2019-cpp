#include "util.h"

#include <fstream>
#include <sstream>

vector<string> read_file(string path) {
  string line;
  vector<string> result;
  ifstream inputfile(path);
  if(inputfile.is_open()) {
    while(getline(inputfile, line)) {
      result.push_back(line);
    }
    inputfile.close();
  }
  return result;
}

vector<string> split_string_to_strings(string input) {
  istringstream ss(input);

  vector<string> data;

  while(ss) {
    string s;
    if(!getline(ss, s, ',')) {
      break;
    }
    data.push_back(s);
  }

  return data;
}


vector<int> split_string_to_ints(string input) {
  vector<string> strings = split_string_to_strings(input);

  vector<int> data;

  for(auto &s : strings) {
    data.push_back(stoi(s));
  }

  return data;
}

vector<long long int> split_string_to_long_longs(string input) {
  vector<string> strings = split_string_to_strings(input);

  vector<long long int> data;

  for(auto &s : strings) {
    data.push_back(stoll(s));
  }

  return data;
}

string& ltrim(string& str, const string& chars) {
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

string& rtrim(string& str, const string& chars) {
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

string& trim(string& str, const string& chars) {
    return ltrim(rtrim(str, chars), chars);
}

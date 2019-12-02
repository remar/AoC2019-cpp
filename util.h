#ifndef UTIL_H
#define UTIL_H

#include <fstream>
#include <string>
#include <vector>

using namespace std;

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

#endif

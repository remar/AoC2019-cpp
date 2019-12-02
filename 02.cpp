#include <iostream>
#include <sstream>

#include "util.h"

using namespace std;

vector<int> get_data(string input) {
  istringstream ss(input);

  vector<int> data;

  while(ss) {
    string s;
    if(!getline(ss, s, ',')) {
      break;
    }
    data.push_back(stoi(s));
  }

  return data;
}

int main() {
  auto lines = read_file("02.txt");

  auto data = get_data(lines[0]);

  int pc = 0;

  data[1] = 12;
  data[2] = 2;

  while(data[pc] != 99) {
    cout << "[PC " << pc << "]" << endl;
    switch(data[pc]) {
    case 1:
      cout << "data[" << data[pc+3] << "] = data["<< data[pc+1] << "] + data[" << data[pc+2] << "]" << endl << flush;
      data[data[pc+3]] = data[data[pc+1]] + data[data[pc+2]];
      break;
    case 2:
      cout << "data[" << data[pc+3] << "] = data["<< data[pc+1] << "] * data[" << data[pc+2] << "]" << endl << flush;
      data[data[pc+3]] = data[data[pc+1]] * data[data[pc+2]];
      break;
    }
    pc += 4;
  }

  cout << data[0] << endl;
}

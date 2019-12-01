#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int fuel_requirement(int mass) {
  int req = mass / 3 - 2;

  if(req <= 0) {
    return 0;
  } else {
    return req + fuel_requirement(req);
  }
}

int main() {
  int sum = 0;

  string line;
  ifstream inputfile("01.txt");
  if(inputfile.is_open()) {
    while(getline(inputfile, line)) {
      sum += fuel_requirement(stoi(line));
    }
    inputfile.close();
  }

  cout << sum << endl;

  return 0;
}

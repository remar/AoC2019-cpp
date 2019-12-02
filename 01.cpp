#include <iostream>

#include "util.h"

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

  auto lines = read_file("01.txt");

  for(auto line : lines) {
    sum += fuel_requirement(stoi(line));
  }

  cout << sum << endl;

  return 0;
}

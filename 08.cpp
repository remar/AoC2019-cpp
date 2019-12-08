#include <iostream>

#include "util.h"

using namespace std;

struct digits {
  int zeros = 0;
  int ones = 0;
  int twos = 0;
};

int main() {
  auto lines = read_file("08.txt");
  cout << lines[0].length()/150 << endl;

  digits layer[100];

  int fewest_zeros = 150;
  int result = -1;

  for(int i = 0;i < 100;i++) {
    layer[i].zeros = layer[i].ones = layer[i].twos = 0;
    for(auto &c : lines[0].substr(i*150, 150)) {
      if(c == '0') {
	layer[i].zeros++;
      } else if(c == '1') {
	layer[i].ones++;
      } else if(c == '2') {
	layer[i].twos++;
      }
    }

    if(layer[i].zeros < fewest_zeros) {
      fewest_zeros = layer[i].zeros;
      result = layer[i].ones * layer[i].twos;
      cout << result << endl;
    }
  }
  return 0;
}

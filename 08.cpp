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

  digits layer[100];

  int fewest_zeros = 150;
  int result = -1;

  constexpr int width = 25;
  constexpr int height = 6;

  int image[width*height];

  for(int i = 0;i < width*height;i++) {
    image[i] = 2;
  }

  for(int i = 0;i < 100;i++) {
    layer[i].zeros = layer[i].ones = layer[i].twos = 0;
    int pos = 0;
    for(auto &c : lines[0].substr(i*150, 150)) {
      int val = -1;
      if(c == '0') {
	val = 0;
      } else if(c == '1') {
	val = 1;
      } else if(c == '2') {
	val = 2;
      }
      if(image[pos] == 2) {
	image[pos] = val;
      }
      ++pos;
    }
  }

  for(int y = 0;y < height;y++) {
    for(int x = 0;x < width;x++) {
      cout << (image[y*width + x] == 0 ? ' ' : 'X');
    }
    cout << endl;
  }

  return 0;
}

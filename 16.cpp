#include <cmath>
#include <iostream>
#include "util.h"

using namespace std;

string real_input() {
  auto lines = read_file("16.txt");
  return lines[0];
}

class PhaseGenerator {
public:
  PhaseGenerator(int phase) : phase {phase}, call {0}, firstCall {true} {}
  int getPhase() {
    if(call == 0) {
      call++;
    }
    int p = pattern[(call / phase) % 4];
    call++;
    return p;
  }

private:
  int phase;
  int call;
  bool firstCall;
  int pattern[4] = {0, 1, 0, -1};
};

vector<int> phase(vector<int> input) {
  vector<int> output;

  int inputSize = input.size();
  int size = inputSize*10000;

  for(int i = 0;i < size;i++) {
    PhaseGenerator pg(i + 1);
    vector<int> totalPhases(inputSize, 0);
    for(int k = 0;k < size;k++) {
      totalPhases[k % inputSize] += pg.getPhase();
    }

    int total = 0;
    for(int j = 0;j < inputSize;j++) {
      total += (input[j] * totalPhases[j]);
    }
    output.push_back(abs(total) % 10);
  }
  return output;
}

int main() {
  string input = "12345678";
  input = "80871224585914546619083218645595";
  input = "19617804207202209144916044189917";
  input = real_input();
  input = "03036732577212944063491565474664";
  cout << input.length() << endl;
  vector<int> signal;

  for(auto ch : input) {
    signal.push_back(ch - 48);
  }

  for(int i = 0;i < 100;i++) {
    signal = phase(signal);
  }

  
  // for(int i = 5972883;i < 5972883+8;i++) {
  //   cout << signal[i];
  // }
  // cout << endl;
}

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
  int size = inputSize;

  for(int i = 0;i < size;i++) {
    PhaseGenerator pg(i + 1);
    int total = 0;
    for(int j = 0;j < inputSize;j++) {
      total += (input[j] * pg.getPhase());
    }
    output.push_back(abs(total) % 10);
  }
  return output;
}

int main() {
  string input = "12345678";
  input = "80871224585914546619083218645595";
  input = "19617804207202209144916044189917";
  input = "03036732577212944063491565474664";
  input = real_input();

  vector<int> signal;

  for(auto ch : input) {
    signal.push_back(ch - 48);
  }

  vector<int> real_signal;
  int signalSize = signal.size();
  for(int i = 0;i < signalSize*10000;i++) {
    real_signal.push_back(signal[i % signalSize]);
  }

  int offset = stoi(input.substr(0,7));

  for(int i = 0;i < 100;i++) {
    signal = phase(signal);
  }
  cout << "First part: ";
  for(int i = 0;i < 8;i++) {
    cout << signal[i];
  }
  cout << endl;

  for(int j = 0;j < 100;j++) {
    int sum = real_signal.back();
    // Overwrite input backwards... and stop at offset
    for(int i = real_signal.size() - 2;i >= offset;i--) {
      sum = (sum + real_signal[i]) % 10;
      real_signal[i] = sum;
    }
  }
  cout << "Second part: ";
  for(int i = offset;i < offset+8;i++) {
    cout << real_signal[i];
  }
  cout << endl;
}

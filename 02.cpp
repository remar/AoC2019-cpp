#include <iostream>

#include "util.h"

using namespace std;

const bool debug = false;

int run_program(vector<int> program) {
  int pc = 0;

  while(program[pc] != 99) {
    if(debug) {
      cout << "[PC " << pc << "]" << endl;
    }
    switch(program[pc]) {
    case 1:
      if(debug) {
	cout << "program[" << program[pc+3] << "] = program["<< program[pc+1] << "] + program[" << program[pc+2] << "]" << endl << flush;
      }
      program[program[pc+3]] = program[program[pc+1]] + program[program[pc+2]];
      break;
    case 2:
      if(debug) {
	cout << "program[" << program[pc+3] << "] = program["<< program[pc+1] << "] * program[" << program[pc+2] << "]" << endl << flush;
      }
      program[program[pc+3]] = program[program[pc+1]] * program[program[pc+2]];
      break;
    }
    pc += 4;
  }

  return program[0];
}

int main() {
  auto lines = read_file("02.txt");

  auto program = split_string_to_ints(lines[0]);

  for(int i = 0;i < 99;i++) {
    for(int j = 0;j < 99;j++) {
      auto program_copy = program;
      program_copy[1] = i;
      program_copy[2] = j;
      if(run_program(program_copy) == 19690720) {
	cout << 100 * i + j << endl;
      }
    }
  }
}

#include <iostream>

#include "util.h"

using namespace std;

const bool debug = true;

enum class mode {
  position,
  immediate
};

struct instruction {
  int opcode;
  mode modes[3];
};

instruction decode(int instr) {
  instruction result;
  result.opcode = instr % 100;
  result.modes[0] = (instr / 100) % 10 == 1 ? mode::immediate : mode::position;
  result.modes[1] = (instr / 1000) % 10 == 1 ? mode::immediate : mode::position;
  result.modes[2] = (instr / 10000) % 10 == 1 ? mode::immediate : mode::position;
  if(debug) {
    cout << "<decode> " << result.opcode << " ("
	 << (result.modes[0] == mode::immediate ? "i" : "p")
	 << (result.modes[1] == mode::immediate ? "i" : "p")
	 << (result.modes[2] == mode::immediate ? "i" : "p")
	 << ")" << endl;
  }
  return result;
}

int run_program(vector<int> program) {
  int pc = 0;
  int cycles = 0;

  while(program[pc]%100 != 99) {
    if(debug) {
      cout << "[PC " << pc << "]" << endl;
    }
    auto instr = decode(program[pc]);
    int op1 = instr.modes[0] == mode::immediate ? pc + 1 : program[pc + 1];
    int op2 = instr.modes[1] == mode::immediate ? pc + 2 : program[pc + 2];
    switch(instr.opcode) {
    case 1: // ADD
      if(debug) {
	cout << "program[" << program[pc+3] << "] = "
	  "program["<< op1 << "] + "
	  "program[" << op2 << "]" << endl << flush;
      }
      program[program[pc+3]] = program[op1] + program[op2];
      pc += 4;
      break;
    case 2: // MUL
      if(debug) {
	cout << "program[" << program[pc+3] << "] = "
	  "program["<< op1 << "] * "
	  "program[" << op2 << "]" << endl << flush;
      }
      program[program[pc+3]] = program[op1] * program[op2];
      pc += 4;
      break;
    case 3: // INPUT
      // read integer from input (always position mode)
      program[program[pc+1]] = 5; // puzzle says input should be '5' for part 2
      pc += 2;
      break;
    case 4: // OUTPUT
      // write integer to output (position or immediate mode)
      cout << "------------------------> " << program[op1] << endl;
      pc += 2;
      break;
    case 5: // JMP if non-zero
      if(program[op1] != 0) {
	if(debug) {
	  cout << "pc = program[" << op2 << "] (" << program[op2] << ")" << endl;
	}
	pc = program[op2];
      } else {
	pc += 3;
      }
      break;
    case 6: // JMP if zero
      if(program[op1] == 0) {
	if(debug) {
	  cout << "pc = program[" << op2 << "] (" << program[op2] << ")" << endl;
	}
	pc = program[op2];
      } else {
	pc += 3;
      }
      break;
    case 7: // LESS THAN
      program[program[pc+3]] = program[op1] < program[op2] ? 1 : 0;
      pc += 4;
      break;
    case 8: // EQUALS
      program[program[pc+3]] = program[op1] == program[op2] ? 1 : 0;
      pc += 4;
      break;
    default:
      throw 0;
      break;
    }

    // if it's a run-away program, abort
    ++cycles;
    if(cycles > 1000) {
      throw 1;
    }
  }

  return program[0];
}

int main() {
  auto lines = read_file("05.txt");

  auto program = split_string_to_ints(lines[0]);

  run_program(program);
}

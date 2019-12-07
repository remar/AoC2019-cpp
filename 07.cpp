#include <iostream>
#include <deque>
#include <algorithm>
#include <string>

#include "util.h"

using namespace std;

constexpr bool debug_puter = false;

class pipe {
public:
  string name;

  int read() {
    int val = buffer.front();
    buffer.pop_front();
    return val;
  }

  void write(int val) {
    buffer.push_back(val);
  }

  void clear() {buffer = {};}

  void print() {
    cout << "[" << name << "] ";
    if(buffer.empty()) {
      cout << "empty";
    } else {
      for(auto x : buffer) {
	cout << x << " ";
      }
    }
    cout << endl;
  }
private:
  deque<int> buffer;
};

class Intcomputer {
public:
  Intcomputer(pipe &input, pipe &output) : input{input}, output{output} {}

  int run_program(vector<int> program) {
    int pc = 0;
    int cycles = 0;

    while(program[pc]%100 != 99) {
      if(debug_puter) {
	cout << "[PC " << pc << "]" << endl;
      }
      auto instr = decode(program[pc]);
      int op1 = instr.modes[0] == mode::immediate ? pc + 1 : program[pc + 1];
      int op2 = instr.modes[1] == mode::immediate ? pc + 2 : program[pc + 2];
      switch(instr.opcode) {
      case 1: // ADD
	if(debug_puter) {
	  cout << "program[" << program[pc+3] << "] = "
	    "program["<< op1 << "] + "
	    "program[" << op2 << "]" << endl << flush;
	}
	program[program[pc+3]] = program[op1] + program[op2];
	pc += 4;
	break;
      case 2: // MUL
	if(debug_puter) {
	  cout << "program[" << program[pc+3] << "] = "
	    "program["<< op1 << "] * "
	    "program[" << op2 << "]" << endl << flush;
	}
	program[program[pc+3]] = program[op1] * program[op2];
	pc += 4;
	break;
      case 3: // INPUT
	// read integer from input (always position mode)
	program[program[pc+1]] = input.read(); // puzzle says input should be '5' for part 2
	pc += 2;
	break;
      case 4: // OUTPUT
	// write integer to output (position or immediate mode)
	output.write(program[op1]);
	if(debug_puter) {
	  cout << "------------------------> " << program[op1] << endl;
	}
	pc += 2;
	break;
      case 5: // JMP if non-zero
	if(program[op1] != 0) {
	  if(debug_puter) {
	    cout << "pc = program[" << op2 << "] (" << program[op2] << ")" << endl;
	  }
	  pc = program[op2];
	} else {
	  pc += 3;
	}
	break;
      case 6: // JMP if zero
	if(program[op1] == 0) {
	  if(debug_puter) {
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

private:
  pipe &input, &output;

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
    if(debug_puter) {
      cout << "<decode> " << result.opcode << " ("
	   << (result.modes[0] == mode::immediate ? "i" : "p")
	   << (result.modes[1] == mode::immediate ? "i" : "p")
	   << (result.modes[2] == mode::immediate ? "i" : "p")
	   << ")" << endl;
    }
    return result;
  }
};

pipe pipes[6];

void clear_pipes() {
  for(auto p : pipes) {
    p.clear();
  }
}

void reset_pipes(int *phase_setting) {
  clear_pipes();
  for(int i = 0;i < 5;i++) {
    pipes[i].write(phase_setting[i]);
  }
  pipes[0].write(0);
  pipes[5].clear();
}

void print_pipes() {
  cout << "Pipes" << endl;
  for(auto &p : pipes) {
    p.print();
  }
}

int main() {
  auto lines = read_file("07.txt");

  auto program = split_string_to_ints(lines[0]);

  Intcomputer puters[5] {
    {pipes[0], pipes[1]},
    {pipes[1], pipes[2]},
    {pipes[2], pipes[3]},
    {pipes[3], pipes[4]},
    {pipes[4], pipes[5]},
  };

  for(int i = 0;i < 6;i++) {
    pipes[i].name = to_string(i);
  }

  int max = 0;

  // all permutations of phase settings
  int phase_setting[] {0, 1, 2, 3, 4};
  do {
    reset_pipes(phase_setting);
    for(auto &puter : puters) {
      puter.run_program(program);
    }

    int output = pipes[5].read();
    if(output > max) {
      max = output;
      cout << max << endl;
    }
  } while(next_permutation(phase_setting, phase_setting+5));
}

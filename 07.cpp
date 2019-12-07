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

  bool input_ready() {
    return !buffer.empty();
  }

  int read() {
    if(buffer.empty()) {
      throw 0;
    }
    int val = buffer.front();
    buffer.pop_front();
    return val;
  }

  void write(int val) {
    buffer.push_back(val);
  }

  void clear() {
    buffer = {};
  }

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
  deque<int> empty;
};

class Intcomputer {
public:
  Intcomputer(pipe &input, pipe &output) : input{input}, output{output} {}

  void load_program(vector<int> program) {
    this->program = program;
    pc = 0;
    cycles = 0;
    halted = false;
  }

  bool is_halted() {return halted;}

  void tick() {
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
      if(input.input_ready()) {
	program[program[pc+1]] = input.read();
	pc += 2;
      }
      // if no input is ready we simply keep the pc at its current
      // position, which means we'll come here again
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
    case 99: // HALT
      halted = true;
      break;
    default:
      throw 0;
      break;
    }

    // if it's a run-away program, abort
    ++cycles;
    if(cycles > 10000) {
      throw 1;
    }
  }

private:
  vector<int> program;
  pipe &input, &output;
  int pc;
  int cycles;
  bool halted;

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

int puters_still_running(Intcomputer *puters) {
  int running = 0;
  for(int i = 0;i < 5;i++) {
    if(!puters[i].is_halted()) {
      running++;
    }
  }
  return running;
}

int main() {
  auto lines = read_file("07.txt");

  auto program = split_string_to_ints(lines[0]);

  Intcomputer puters[5] {
    {pipes[0], pipes[1]},
    {pipes[1], pipes[2]},
    {pipes[2], pipes[3]},
    {pipes[3], pipes[4]},
    {pipes[4], pipes[0]},
  };

  for(int i = 0;i < 6;i++) {
    pipes[i].name = to_string(i);
  }

  int max = 0;

  // all permutations of phase settings
  int phase_setting[] {5, 6, 7, 8, 9};
  do {
    reset_pipes(phase_setting);
    for(auto &puter : puters) {
      puter.load_program(program);
    }
    while(puters_still_running(puters) > 0) {
      for(auto &puter : puters) {
	puter.tick();
      }
    }
    int output = pipes[0].read();
    if(output > max) {
      max = output;
      cout << max << endl;
    }
  } while(next_permutation(phase_setting, phase_setting+5));
}

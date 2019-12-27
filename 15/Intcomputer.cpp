#include "Intcomputer.h"

#include <deque>
#include <iostream>

#include "util.h"

using namespace std;

Intcomputer::Intcomputer(Pipe &input, Pipe &output) : input{input}, output{output} {}

void Intcomputer::load_program(vector<long long int> program) {
  this->program = program;
  this->program.resize(100000);
  pc = 0;
  cycles = 0;
  halted = false;
  index = 0;
}

void Intcomputer::load_program_from_file(string path) {
  auto lines = read_file(path);
  load_program(split_string_to_long_longs(lines[0]));
}

void Intcomputer::write_value(int address, long long value) {
  program[address] = value;
}

bool Intcomputer::is_halted() {return halted;}

void Intcomputer::run() {
  do {
    tick();
  } while(!is_halted());
  if(debug) {
    cout << "Cycles: " << cycles << endl;
  }
}

void Intcomputer::tick() {
  if(debug) {
    cout << "[PC " << pc << "]" << endl;
  }
  auto instr = decode(program[pc]);
  int op1 = get_param(instr.modes[0], 1);
  int op2 = get_param(instr.modes[1], 2);
  int op3 = get_param(instr.modes[2], 3);
  switch(instr.opcode) {
  case 1: // ADD
    if(debug) {
      cout << "program[" << op3 << "] = "
	"program["<< op1 << "] + "
	"program[" << op2 << "]" << endl << flush;
    }
    program[op3] = program[op1] + program[op2];
    pc += 4;
    break;
  case 2: // MUL
    if(debug) {
      cout << "program[" << op3 << "] = "
	"program["<< op1 << "] * "
	"program[" << op2 << "]" << endl << flush;
    }
    program[op3] = program[op1] * program[op2];
    pc += 4;
    break;
  case 3: // INPUT
    // read integer from input (always position mode)
    if(input.input_ready()) {
      program[op1] = input.read();
      pc += 2;
    }
    // if no input is ready we simply keep the pc at its current
    // position, which means we'll come here again
    break;
  case 4: // OUTPUT
    // write integer to output (position or immediate mode)
    output.write(program[op1]);
    if(debug) {
      cout << "------------------------> " << program[op1] << endl;
    }
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
    program[op3] = program[op1] < program[op2] ? 1 : 0;
    pc += 4;
    break;
  case 8: // EQUALS
    program[op3] = program[op1] == program[op2] ? 1 : 0;
    pc += 4;
    break;
  case 9: // Set INDEX
    index += program[op1];
    pc += 2;
    break;
  case 99: // HALT
    halted = true;
    break;
  default:
    cout << "Unknown instr " << instr.opcode << endl << flush;
    throw 0;
    break;
  }

  // if it's a run-away program, abort
  ++cycles;
  if(cycles > 1000000000) {
    cout << "Runaway program!" << endl << flush;
    throw 1;
  }
}

long long int Intcomputer::get_param(mode m, int offset) {
  switch(m) {
  case mode::position: return program[pc + offset]; break;
  case mode::immediate: return pc + offset; break;
  case mode::relative: return index + program[pc + offset]; break;
  }

  throw 2;
}

Intcomputer::instruction Intcomputer::decode(int instr) {
  instruction result;
  result.opcode = instr % 100;
  result.modes[0] = static_cast<mode>((instr / 100) % 10);
  result.modes[1] = static_cast<mode>((instr / 1000) % 10);
  result.modes[2] = static_cast<mode>((instr / 10000) % 10);
  if(debug) {
    cout << "<decode> " << result.opcode << " ("
	 << (result.modes[0] == mode::immediate ? "i" : "p")
	 << (result.modes[1] == mode::immediate ? "i" : "p")
	 << (result.modes[2] == mode::immediate ? "i" : "p")
	 << ")" << endl;
  }
  return result;
}

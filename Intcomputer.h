#include <deque>
#include <iostream>

using namespace std;

class pipe {
public:
  string name;

  bool input_ready() {
    return !buffer.empty();
  }

  int buffer_length() {
    return buffer.size();
  }

  long long int read() {
    if(buffer.empty()) {
      throw 0;
    }
    long long int val = buffer.front();
    buffer.pop_front();
    return val;
  }

  void write(long long int val) {
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
  deque<long long int> buffer;
  deque<long long int> empty;
};

class Intcomputer {
public:
  Intcomputer(pipe &input, pipe &output) : input{input}, output{output} {}

  void load_program(vector<long long int> program) {
    this->program = program;
    this->program.resize(100000);
    pc = 0;
    cycles = 0;
    halted = false;
    index = 0;
  }

  bool is_halted() {return halted;}

  void run() {
    do {
      tick();
    } while(!is_halted());
    if(debug) {
      cout << "Cycles: " << cycles << endl;
    }
  }

  void tick() {
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
    if(cycles > 10000000) {
      cout << "Runaway program!" << endl << flush;
      throw 1;
    }
  }

private:
  vector<long long int> program;
  pipe &input, &output;
  int pc;
  int cycles;
  bool halted;
  int index;
  bool debug = false;

  enum class mode {
    position = 0,
    immediate = 1,
    relative = 2
  };

  struct instruction {
    int opcode;
    mode modes[3];
  };

  long long int get_param(mode m, int offset) {
    switch(m) {
    case mode::position: return program[pc + offset]; break;
    case mode::immediate: return pc + offset; break;
    case mode::relative: return index + program[pc + offset]; break;
    }

    throw 2;
  }

  instruction decode(int instr) {
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
};

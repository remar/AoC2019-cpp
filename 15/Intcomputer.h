#ifndef INTCOMPUTER_H
#define INTCOMPUTER_H

#include <vector>
#include "Pipe.h"

using namespace std;

class Intcomputer {
public:
  Intcomputer(Pipe &input, Pipe &output);
  void load_program(vector<long long int> program);
  void load_program_from_file(string path);
  void write_value(int address, long long value);
  bool is_halted();
  void run();
  void tick();

private:
  vector<long long int> program;
  Pipe &input, &output;
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

  long long int get_param(mode m, int offset);
  instruction decode(int instr);
};

#endif

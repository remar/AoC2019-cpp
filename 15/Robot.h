#ifndef ROBOT_H
#define ROBOT_H

#include "Intcomputer.h"

class Robot {
public:
  Robot();

  enum class MoveResult {Wall, Open, Goal, Unknown};
  MoveResult move(int direction);
  MoveResult reverse(int direction);
  vector<MoveResult> check_directions();

private:
  Pipe input, output;
  Intcomputer puter;
};

#endif

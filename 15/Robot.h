#ifndef ROBOT_H
#define ROBOT_H

#include "Intcomputer.h"

class Robot {
public:
  Robot();
  vector<bool> check_directions();
  int move(int direction);
  void draw_surroundings(int x, int y);

private:
  Pipe input, output;
  Intcomputer puter;
};

#endif

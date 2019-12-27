#include "Robot.h"
#include "Cats.h"
#include <map>

using namespace std;

struct point {
  int x, y;
};

map<int,point> dir_map = {
  {0, {0, 0}},
  {1, {0, -1}},
  {2, {0, +1}},
  {3, {-1, 0}},
  {4, {+1, 0}},
};

Robot::Robot() : puter {input, output} {
  input.name = "input";
  output.name = "output";
  puter.load_program_from_file("15.txt");
}


vector<bool> Robot::check_directions() {
  int move_back[] = {0, 2, 1, 4, 3};
  vector<bool> openings {false};
  // go in each direction, checking for openings
  for(int i = 1;i <= 4;i++) {
    bool opening = (move(i) != 0);
    if(opening) {
      // Move back!
      move(move_back[i]);
    }
    openings.push_back(opening);
  }
  return openings;
}

int Robot::move(int direction) {
  input.write(direction);
  while(!output.input_ready()) {
    puter.tick();
  }
  return output.read();
}


void Robot::draw_surroundings(int x, int y) {
  auto directions = check_directions();
  for(int i = 1;i <= 4;i++) {
    Cats::SetTile(x + dir_map[i].x, y + dir_map[i].y, "tiles", directions[i] ? 0 : 1, 0);
  }
  Cats::SetTile(x, y, "tiles", 4, 0);
}

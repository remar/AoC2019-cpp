#include "Robot.h"
#include "Cats.h"

using namespace std;

Robot::Robot() : puter {input, output} {
  input.name = "input";
  output.name = "output";
  puter.load_program_from_file("15.txt");
}


vector<Robot::MoveResult> Robot::check_directions() {
  vector<Robot::MoveResult> moveResults {Robot::MoveResult::Unknown};
  // go in each direction, checking for openings
  for(int i = 1;i <= 4;i++) {
    moveResults.push_back(move(i));
    if(moveResults.back() != Robot::MoveResult::Wall) {
      // Move back!
      reverse(i);
    }
  }
  return moveResults;
}

Robot::MoveResult Robot::move(int direction) {
  input.write(direction);
  while(!output.input_ready()) {
    puter.tick();
  }
  return Robot::MoveResult(output.read());
}

Robot::MoveResult Robot::reverse(int direction) {
  int move_back[] = {0, 2, 1, 4, 3};
  return move(move_back[direction]);
}

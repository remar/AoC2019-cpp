#include <iostream>
#include <map>
#include <SDL2/SDL.h>
#include "Input.h"
#include "Robot.h"
#include "Cats.h"
#include <list>

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

enum class Tile {unknown, open, wall, oxygen};

constexpr int width = 50;
constexpr int height = 50;
constexpr int tileWidth = 4;
constexpr int tileHeight = 4;

Tile field[width][height];

int robot_x, robot_y;

void clear_field() {
  for(int y = 0;y < height;y++) {
    for(int x = 0;x < width;x++) {
      field[x][y] = Tile::unknown;
    }
  }
}

Tile move_result_to_tile(Robot::MoveResult result) {
  switch(result) {
  case Robot::MoveResult::Wall:
    return Tile::wall;
  case Robot::MoveResult::Open:
    return Tile::open;
  case Robot::MoveResult::Goal:
    return Tile::oxygen;
  case Robot::MoveResult::Unknown:
    return Tile::unknown;
  }
}

void update_field(vector<Robot::MoveResult> directions, int x, int y) {
  for(int i = 1;i <= 4;i++) {
    field[x + dir_map[i].x][y + dir_map[i].y] = move_result_to_tile(directions[i]);
  }
}

bool has_neighbour(int x, int y, Tile tile) {
  for(int i = 1;i <=4;i++) {
    if(field[x + dir_map[i].x][y + dir_map[i].y] == tile) {
      return true;
    }
  }
  return false;
}

bool has_unknown_neighbour(int x, int y) {
  return has_neighbour(x, y, Tile::unknown);
}

bool has_open_neighbour(int x, int y) {
  return has_neighbour(x, y, Tile::open);
}

bool has_oxygen_neighbour(int x, int y) {
  return has_neighbour(x, y, Tile::oxygen);
}

void draw() {
  for(int y = 0;y < height;y++) {
    for(int x = 0;x < width;x++) {
      if(field[x][y] == Tile::unknown && has_open_neighbour(x, y)) {
	Cats::SetTile(x, y, "tiles", (int)field[x][y], 0);
      } else if(field[x][y] != Tile::unknown) {
	Cats::SetTile(x, y, "tiles", (int)field[x][y], 0);
      }
    }
  }
  // Draw robot
  Cats::SetTile(robot_x, robot_y, "tiles", 4, 0);
}

vector<point> find_frontier() {
  vector<point> result;
  for(int y = 1;y < height - 1;y++) {
    for(int x = 1;x < width - 1;x++) {
      if(field[x][y] == Tile::open) {
	if(has_unknown_neighbour(x, y)) {
	  result.push_back(point {x, y});
	}
      }
    }
  }
  return result;
}

int reachable_from(vector<point> frontier, int robot_x, int robot_y) {
  for(int f = 0;f < frontier.size();f++) {
    for(int i = 1;i <= 4;i++) {
      if(frontier[f].x == robot_x + dir_map[i].x
	 && frontier[f].y == robot_y + dir_map[i].y) {
	return f;
      }
    }
  }
  return -1;
}

int get_robot_movement(point move_to, int robot_x, int robot_y) {
  int dx = move_to.x - robot_x;
  int dy = move_to.y - robot_y;
  for(int i = 1;i <= 4;i++) {
    if(dir_map[i].x == dx && dir_map[i].y == dy) {
      return i;
    }
  }
  throw 0;
}

vector<point> get_oxygen_spread() {
  vector<point> result;
  for(int y = 1;y < height - 1;y++) {
    for(int x = 1;x < width - 1;x++) {
      if(field[x][y] == Tile::open && has_oxygen_neighbour(x, y)) {
	result.push_back(point {x, y});
      }
    }
  }
  return result;
}

int main() {
  Robot robot;

  clear_field();

  Cats::Init(width*tileWidth, height*tileHeight, 3);

  Cats::LoadTileset("tiles.json");
  Cats::SetupTileLayer(width, height, tileWidth, tileHeight);

  int lastFrameTime = SDL_GetTicks();

  robot_x = width/2;
  robot_y = height/2;
  field[robot_x][robot_y] = Tile::open;

  update_field(robot.check_directions(), robot_x, robot_y);
  draw();

  list<int> moves;
  
  while(!Input::instance.gotQuitEvent()) {
    Input::instance.update();

    for(int j = 0;j < 10;j++) {
      auto frontier = find_frontier();
      int i = reachable_from(frontier, robot_x, robot_y);
      if(i != -1) {
	int m = get_robot_movement(frontier[i], robot_x, robot_y);

	moves.push_front(m);

	robot.move(m);
	robot_x += (frontier[i].x - robot_x);
	robot_y += (frontier[i].y - robot_y);
	update_field(robot.check_directions(), robot_x, robot_y);
	draw();
      } else {
	// backtrack
	if(moves.empty()) {
	  break;
	}
	int m = moves.front();
	moves.pop_front();
	robot.reverse(m);
	robot_x -= dir_map[m].x;
	robot_y -= dir_map[m].y;
	draw();
      }
    }

    if(moves.empty()) {
      draw();
      Cats::Redraw(0);
      break;
    }

    Cats::Redraw(0);
  }

  int minutes = 0;
  while(!Input::instance.gotQuitEvent()) {
    Input::instance.update();
    vector<point> spread = get_oxygen_spread();
    if(spread.empty()) {
      cout << "Minutes: " << minutes << endl;
      break;
    }
    for(auto s : spread) {
      field[s.x][s.y] = Tile::oxygen;
    }
    minutes++;
    draw();
    Cats::Redraw(0);
  }

  return 0;
}

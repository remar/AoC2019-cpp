#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <unordered_map>

#include "util.h"

struct LineValue {
  int steps;
};

struct Point {
  int x, y;

  bool operator==(const Point &other) const {
    return x == other.x && y == other.y;
  }
};

namespace std {
  template <>
  struct hash<Point>
  {
    std::size_t operator()(const Point& k) const
    {
      return ((hash<int>()(k.x)
               ^ (hash<int>()(k.y) << 1)) >> 1);
    }
  };
}

unordered_map<Point, LineValue> put_wire(vector<string> wire) {
  unordered_map<Point, LineValue> field;

  map<char, Point> delta = {
    {'U', {0, 1}},
    {'D', {0, -1}},
    {'L', {-1, 0}},
    {'R', {1, 0}},
  };

  int stepsTaken = 0;
  Point pos {0, 0};
  for(auto command : wire) {
    char direction = command[0];
    int len = stoi(command.substr(1));

    // move in direction len spaces
    for(int i = 0;i < len;i++) {
      ++stepsTaken;

      pos.x += delta[direction].x;
      pos.y += delta[direction].y;
      field.insert({pos, {stepsTaken}});
    }
  }

  return field;
}

int main() {
  auto lines = read_file("03.txt");

  vector<string> first_wire = split_string_to_strings(lines[0]);

  vector<string> second_wire = split_string_to_strings(lines[1]);

  auto first_field = put_wire(first_wire);
  auto second_field = put_wire(second_wire);

  // Locate crossings
  int closest = numeric_limits<int>::max();
  int fewest_steps = numeric_limits<int>::max();

  for(auto pos : first_field) {
    if(second_field.find(pos.first) != second_field.end()) {
      int manhattan = abs(pos.first.x) + abs(pos.first.y);
      if(manhattan < closest) {
	closest = manhattan;
      }
      int steps = pos.second.steps + second_field[pos.first].steps;
      if(steps < fewest_steps) {
	fewest_steps = steps;
      }
    }
  }
  cout << closest << endl;
  cout << fewest_steps << endl;
}

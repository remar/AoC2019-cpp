#include <vector>
#include "util.h"
#include "Intcomputer.h"

bool debug = false;

enum class color {
  black = 0,
  white = 1
};

enum direction {
  up = 0,
  right = 1,
  down = 2,
  left = 3
};

struct point {
  int x, y;
  color c;
};

vector<point> field;

void print_field() {
  for(auto p : field) {
    cout << p.x << "," << p.y << ": " << static_cast<int>(p.c) << endl;
  }
}

color color_at(int x, int y) {
  for(auto &p : field) {
    if(p.x == x && p.y == y) {
      return p.c;
    }
  }
  return color::black;
}

void print_message() {
  int min_x = 10000, max_x = -10000, min_y = 10000, max_y = -10000;
  for(auto &p : field) {
    if(p.x < min_x) {
      min_x = p.x;
    }
    if(p.x > max_x) {
      max_x = p.x;
    }
    if(p.y < min_y) {
      min_y = p.y;
    }
    if(p.y > max_y) {
      max_y = p.y;
    }
  }

  cout << "min_x: " << min_x << ", max_x: " << max_x << endl;
  cout << "min_y: " << min_y << ", max_y: " << max_y << endl;

  color bitmap[43][6];
  for(int y = 0;y < 6;y++) {
    for(int x = 0;x < 43;x++) {
      bitmap[x][y] = color::black;
    }
  }

  for(auto &p : field) {
    bitmap[p.x][-p.y] = p.c;
  }

  for(int y = 0;y < 6;y++) {
    for(int x = 0;x < 43;x++) {
      cout << (bitmap[x][y] == color::white ? "X" : " ");
    }
    cout << endl;
  }
}

int main() {
  auto lines = read_file("11.txt");
  auto program = split_string_to_long_longs(lines[0]);

  pipe input, output;
  Intcomputer puter(input, output);

  int x = 0, y = 0;
  color c = color::black;
  int dir = up;

  puter.load_program(program);

  input.write(static_cast<int>(color::white));

  while(!puter.is_halted()) {
    while(output.buffer_length() < 2) {
      try {
	puter.tick();
      } catch(int e) {
	print_message();
	return 0;
      }
    }

    c = static_cast<color>(output.read());

    if(debug) {
      cout << (c == color::white ? "white" : "black") << endl;
    }

    bool found = false;
    for(int i = 0;i < field.size();i++) {
      if(field[i].x == x && field[i].y == y) {
	field[i].c = c;
	found = true;
	break;
      }
    }
    if(!found) {
      field.push_back({x, y, color::black});
      field[field.size() - 1].c = c;
    }

    if(debug) {
      print_field();
    }

    dir = (dir + (output.read() == 0 ? -1 : 1)) % 4;
    while(dir < 0) {
      dir += 4;
    }
    switch(dir) {
    case direction::up: y++; break;
    case direction::right: x++; break;
    case direction::down: y--; break;
    case direction::left: x--; break;
    }

    input.write(static_cast<int>(color_at(x, y)));
  }

  output.print();

  return 0;
}

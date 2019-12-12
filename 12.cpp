#include <iostream>
#include <cmath>

using namespace std;

struct vec {
  int x, y, z;
};

struct moon {
  vec pos;
  vec vel;
};

void print_moon(moon &m) {
  cout << "pos=<x=" << m.pos.x << ", y=" << m.pos.y << ", z=" << m.pos.z << ">, <x="
       << m.vel.x << ", y=" << m.vel.y << ", z=" << m.vel.z << ">" << endl;
}

int get_direction(int target, int dragger) {
  if(dragger < target) {
    return -1;
  } else if(dragger > target) {
    return +1;
  }
  return 0;
}

void update_vel(moon &target, moon &dragger) {
  target.vel.x += get_direction(target.pos.x, dragger.pos.x);
  target.vel.y += get_direction(target.pos.y, dragger.pos.y);
  target.vel.z += get_direction(target.pos.z, dragger.pos.z);
}

void update_pos(moon &m) {
  m.pos.x += m.vel.x;
  m.pos.y += m.vel.y;
  m.pos.z += m.vel.z;
}

int energy(vec &v) {
  return abs(v.x) + abs(v.y) + abs(v.z);
}

int total_energy(moon &m) {
  return energy(m.pos) * energy(m.vel);
}

int main() {
  moon moons[4] = {
    {{17, -9, 4}, {0,0,0}}, // <x=17, y=-9, z=4>
    {{2, 2, -13}, {0,0,0}}, // <x=2, y=2, z=-13>
    {{-1, 5, -1}, {0,0,0}}, // <x=-1, y=5, z=-1>
    {{4, 7, -7},  {0,0,0}}, // <x=4, y=7, z=-7>

    // {{-1, 0, 2}, {0,0,0}},
    // {{2, -10, -7}, {0,0,0}},
    // {{4, -8, 8}, {0,0,0}},
    // {{3, 5, -1},  {0,0,0}},
  };

  cout << "After 0 steps" << endl;
  for(int i = 0;i < 4;i++) {
    print_moon(moons[i]);
  }
  cout << endl;

  for(int iter = 0;iter < 1000;iter++) {
    for(int i = 0;i < 4;i++) {
      for(int j = 0;j < 4;j++) {
	if(i == j) {
	  continue;
	}
	update_vel(moons[i], moons[j]);
      }
    }

    for(int i = 0;i < 4;i++) {
      update_pos(moons[i]);
    }

    cout << "After " << iter+1 << " steps" << endl;
    for(int i = 0;i < 4;i++) {
      print_moon(moons[i]);
    }
    cout << endl;
  }

  int total = 0;
  for(int i = 0;i < 4;i++) {
    total += total_energy(moons[i]);
  }
  cout << "total: " << total << endl;
  return 0;
}


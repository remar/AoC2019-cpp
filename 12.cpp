#include <iostream>
#include <cmath>
#include <cstring>
#include <vector>

using namespace std;

struct vec_short {
  short x, y, z;
};

struct vec_char {
  char x, y, z;
};

struct moon {
  vec_short pos;
  vec_char vel;
};

struct moon_axis {
  int pos;
  int vel;
};

void print_moon(moon &m) {
  cout << "pos=<x=" << m.pos.x << ", y=" << m.pos.y << ", z=" << m.pos.z << ">, <x="
       << (int)m.vel.x << ", y=" << (int)m.vel.y << ", z=" << (int)m.vel.z << ">" << endl;
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


void update_vel(moon_axis &target, moon_axis &dragger) {
  target.vel += get_direction(target.pos, dragger.pos);
}

void update_pos(moon &m) {
  m.pos.x += m.vel.x;
  m.pos.y += m.vel.y;
  m.pos.z += m.vel.z;
}

void update_pos(moon_axis &m) {
  m.pos += m.vel;
}

int energy(vec_short &v) {
  return abs(v.x) + abs(v.y) + abs(v.z);
}

int energy(vec_char &v) {
  return abs(v.x) + abs(v.y) + abs(v.z);
}

int total_energy(moon &m) {
  return energy(m.pos) * energy(m.vel);
}

struct moons {
  moon m[4] = {
    {{17, -9, 4}, {0,0,0}}, // <x=17, y=-9, z=4>
    {{2, 2, -13}, {0,0,0}}, // <x=2, y=2, z=-13>
    {{-1, 5, -1}, {0,0,0}}, // <x=-1, y=5, z=-1>
    {{4, 7, -7},  {0,0,0}}, // <x=4, y=7, z=-7>

    // {{-1, 0, 2}, {0,0,0}},
    // {{2, -10, -7}, {0,0,0}},
    // {{4, -8, 8}, {0,0,0}},
    // {{3, 5, -1},  {0,0,0}},
  };

  void update() {
    for(int i = 0;i < 4;i++) {
      for(int j = 0;j < 4;j++) {
	if(i == j) {
	  continue;
	}
	update_vel(m[i], m[j]);
      }
    }

    for(int i = 0;i < 4;i++) {
      update_pos(m[i]);
    }
  }

  int energy() {
    int total = 0;
    for(int i = 0;i < 4;i++) {
      total += total_energy(m[i]);
    }
    return total;
  }

  void print(int iter) {
    cout << "After " << iter+1 << " iterations" << endl;
    for(int i = 0;i < 4;i++) {
      print_moon(m[i]);
    }
  }
};

long long int cycles(int m1, int m2, int m3, int m4) {
  moon_axis m[4] = {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
  };
  m[0].pos = m1;
  m[1].pos = m2;
  m[2].pos = m3;
  m[3].pos = m4;
  moon_axis orig[4];

  for(int i = 0;i < 4;i++) {
    orig[i] = m[i];
  }

  for(int iter = 0;iter < 1000000;iter++) {
    for(int i = 0;i < 4;i++) {
      for(int j = 0;j < 4;j++) {
	if(i == j) {
	  continue;
	}
	update_vel(m[i], m[j]);
      }
    }
    for(int i = 0;i < 4;i++) {
      update_pos(m[i]);
    }
    int matches = 0;
    for(int i = 0;i < 4;i++) {
      if(m[i].pos == orig[i].pos && m[i].vel == orig[i].vel) {
	matches++;
      }
    }
    if(matches == 4) {
      return iter + 1;
    }
  }
}

long long int gcd(int a, int b) {
  if(b > a) {
    swap(a, b);
  }

  while(b != 0) {
    int t = b;
    b = a % b;
    a = t;
  }

  return a;
}

int main() {
  moon_axis mx[4] = {
    {17, 0},
    {2, 0},
    {-1, 0},
    {4, 0},
  };

  moon_axis my[4] = {
    {-9, 0},
    {2, 0},
    {5, 0},
    {7, 0},
  };

  moon_axis mz[4] = {
    {4, 0},
    {-13, 0},
    {-1, 0},
    {-7, 0},
  };

  long long int cycles_x = cycles(17, 2, -1, 4);
  long long int cycles_y = cycles(-9, 2, 5, 7);
  long long int cycles_z = cycles(4, -13, -1, -7);

  long long int lcm = (cycles_x * cycles_y) / gcd(cycles_x, cycles_y);

  lcm = (lcm * cycles_z) / gcd(lcm, cycles_z);

  cout << lcm << endl;

  return 0;
}

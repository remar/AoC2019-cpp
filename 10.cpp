#include <algorithm>
#include <cmath>
#include <iostream>
#include "util.h"

using namespace std;

int gcd(int a, int b) {
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

struct point {
  int x, y;
};

vector<point> roids;


bool asteroid_at(int x, int y) {
  for(auto &r : roids) {
    if(r.x == x && r.y == y) {
      return true;
    }
  }
  return false;
}

vector<point> reachable_from(point from) {
  vector<point> result;

  for(auto &to : roids) {
    if(from.x == to.x && from.y == to.y) {
      continue;
    }

    int dx = to.x - from.x;
    int dy = to.y - from.y;
    int common = gcd(abs(dx), abs(dy));
    if(common > 0) {
      dx /= common;
      dy /= common;
    }

    int x = from.x;
    int y = from.y;

    // go by adding dx, dy, checking for matches along the way
    for(;;) {
      x += dx;
      y += dy;
      if(x == to.x && y == to.y) {
	result.push_back(to);
	// we're at our destination
	break;
      }
      if(asteroid_at(x, y)) {
	// blocked at x, y
	break;
      }
    }
  }

  return result;
}

int main() {
  auto lines = read_file("10_5.txt");

  for(int y = 0;y < lines.size();y++) {
    auto &line = lines[y];
    for(int x = 0;x < line.length();x++) {
      if(line[x] == '#') {
	roids.push_back(point {x, y});
      }
    }
  }

  int max_los = 0;
  point best;
  vector<point> reachable_from_best;

  for(auto &from : roids) {
    auto v = reachable_from(from);

    if(v.size() > max_los) {
      best = from;
      reachable_from_best = v;
      max_los = v.size();
    }
  }
  cout << best.x << "," << best.y << endl;

  vector<point> upper_right;
  for(auto r : reachable_from_best) {
    if(r.x >= best.x && r.y < best.y) {
      upper_right.push_back(r);
    }
  }

  for(auto r : upper_right) {
    cout << r.x << "," << r.y << ":";
  }
  cout << endl;

  sort(upper_right.begin(), upper_right.end(),
       [&](const point &a, const point &b) {
	 float a_diff = ((float)(a.x - best.x))/((float)(best.y - a.y));
	 float b_diff = ((float)(b.x - best.x))/((float)(best.y - b.y));
	 return a_diff < b_diff;
       });
  for(auto r : upper_right) {
    cout << r.x << "," << r.y << ":";
  }
  cout << endl;
}

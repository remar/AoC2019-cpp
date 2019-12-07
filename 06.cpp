#include <iostream>
#include <vector>
#include <unordered_map>

#include "util.h"

using namespace std;

class object {
public:
  object(string name) : name{name}, orbiting_around {nullptr} {}
  string name;
  object *orbiting_around;
};

class obj_container {
public:
  object *get(string x) {
    if(objects.find(x) == objects.end()) {
      objects.emplace(x, new object(x));
    }
    return objects.at(x);
  }

  int total_orbits() {
    int total = 0;
    for(auto obj : objects) {
      total += total_orbits_for(obj.second);
    }
    return total;
  }

private:
  unordered_map<string,object *> objects;

  int total_orbits_for(object *obj) {
    int result = 0;
    while(obj->orbiting_around != nullptr) {
      ++result;
      obj = obj->orbiting_around;
    }
    return result;
  }
};

int main() {
  auto lines = read_file("06.txt");

  auto orbit_spec = split_string_to_strings(lines[0]);

  obj_container objects;

  for(auto spec : orbit_spec) {
    auto parent = objects.get(spec.substr(0,3));
    auto child = objects.get(spec.substr(4,3));
    child->orbiting_around = parent;
  }

  cout << objects.total_orbits() << endl;
}

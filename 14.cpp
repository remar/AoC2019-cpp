#include <iostream>
#include <map>

#include "util.h"

using namespace std;

bool debug_parsing = false;

class ingredient {
public:
  ingredient() {}
  ingredient(string spec) {
    set(spec);
  }

  void set(string spec) {
    int index = spec.find(" ");
    amount = stoi(spec.substr(0, index));
    name = spec.substr(index+1);
    if(debug_parsing) {
      cout << "(" << amount << ") '" << name << "'" << endl;
    }
  }

  int amount;
  string name;
};

class recipe {
public:
  recipe();
  recipe(string spec) {
    if(debug_parsing) {
      cout << "Parsing '" << spec << "'" << endl;
    }
    int index = spec.find("=>");
    string ingredient_list = spec.substr(0, index);
    for(auto &i : split_string_to_strings(ingredient_list)) {
      ingredient ingr(trim(i));
      ingredients.push_back(ingr);
    }

    string s = spec.substr(index+3);
    result.set(s);
  }

  bool is_recipe_for(string element) {
    return result.name == element;
  }

  void print() {
    for(auto &i : ingredients) {
      cout << i.amount << " " << i.name << ", ";
    }
    cout << "=> " << result.amount << " " << result.name << endl;
  }

  vector<ingredient> ingredients;
  ingredient result;
};

class factory;

map<string, factory*> factories;

int total_ore = 0;

class factory {
public:
  factory(recipe r) : name {r.result.name}, prod {r}, stock {0}, request_to_produce {false} {
    for(auto &ingr : prod.ingredients) {
      needed[ingr.name] = ingr.amount;
      internal_stock[ingr.name] = 0;
    }
  }

  string get_name() {
    return name;
  }

  recipe get_recipe() {
    return prod;
  }

  int request(int amount) {
    if(stock >= amount) {
      // give back just as much as is wanted
      stock -= amount;
      return amount;
    } else {
      amount = stock;
      stock = 0;
      request_to_produce = true;
      return amount;
    }
  }

  void produce() {
    // cout << name << " produce! request_to_produce?: "
    // 	 << (request_to_produce ? "true" : "false") << endl;
    if(request_to_produce) {
      request_to_produce = false;

      if(prod.ingredients.size() == 1 && prod.ingredients[0].name == "ORE") {
	// cout << "Produce ORE!!" << endl;
	stock += prod.result.amount;
	total_ore += prod.ingredients[0].amount;
      } else {
	for(auto &ingr : prod.ingredients) {
	  int need = needed[ingr.name] - internal_stock[ingr.name];
	  if(need > 0) {
	    internal_stock[ingr.name] += factories[ingr.name]->request(need);
	  }
	}
	bool has_enough_stock = true;
	for(auto &ingr : prod.ingredients) {
	  if(internal_stock[ingr.name] < needed[ingr.name]) {
	    has_enough_stock = false;
	    break;
	  }
	}
	if(has_enough_stock) {
	  for(auto &ingr : prod.ingredients) {
	    internal_stock[ingr.name] -= needed[ingr.name];
	  }
	  stock += prod.result.amount;
	}
      }
    }
  }

  void print() {
    cout << "Factory " << name << ", producing ";
    for(auto &in : internal_stock) {
      cout << needed[in.first] << " (" << internal_stock[in.first] << ") " << in.first << ", ";
    }
    cout << "=> " << prod.result.amount << " (" << stock << ") " << prod.result.name << endl;
  }
private:
  string name;
  recipe prod;
  int stock;
  map<string,int> internal_stock;
  map<string,int> needed;
  bool request_to_produce;
};

int main() {
  auto lines = read_file("14.txt");

  vector<recipe> recipes;

  factory *fuel_factory;

  for(auto &l : lines) {
    recipe r {l};
    factory *f = new factory(r);
    factories[f->get_name()] = f;
    f->print();
    if(f->get_name() == "FUEL") {
      fuel_factory = f;
    }
  }

  fuel_factory->request(1);

  while(fuel_factory->request(1) < 1) {
    for(auto f : factories) {
      f.second->produce();
      // f.second->print();
    }
  }

  cout << "total ore: " << total_ore << endl;
}

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

  long long int amount;
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

long long int total_ore = 0;

long long int ore = 1000000000000;
long long int fuel_created = 0;

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

  // produce 'amount' number of this element
  long long int produce(long long int amount = 0) {
    // call each subsequent factory to be able to produce 'amount' of
    // this element
    long long int times = ((amount-stock) / prod.result.amount) + ((amount-stock) % prod.result.amount == 0 ? 0 : 1);

    if(is_ore_factory()) {
      if(stock > amount) {
	stock -= amount;
	return amount;
      }
      total_ore += prod.ingredients[0].amount * times;
      stock += prod.result.amount * times - amount;
      return amount;
    } else {
      for(auto &ingr : prod.ingredients) {
	long long int need = needed[ingr.name] * times - internal_stock[ingr.name];
	if(need > 0) {
	  internal_stock[ingr.name] += factories[ingr.name]->produce(need);
	}
      }
      for(auto &ingr : prod.ingredients) {
	internal_stock[ingr.name] -= needed[ingr.name] * times;
      }
      stock += (prod.result.amount * times - amount);
      return amount;
    }
  }

  bool is_ore_factory() {
    return prod.ingredients.size() == 1 && prod.ingredients[0].name == "ORE";
  }

  void print() {
    cout << "Factory " << name << ", producing ";
    for(auto &in : internal_stock) {
      cout << needed[in.first] << " (" << internal_stock[in.first] << ") " << in.first << ", ";
    }
    cout << "=> " << prod.result.amount << " (" << stock << ") " << prod.result.name << endl;
  }

  int peek_stock() {
    return stock;
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
    if(f->get_name() == "FUEL") {
      fuel_factory = f;
    }
  }

  fuel_factory->produce(3279311);

  cout << "total_ore: " << total_ore << endl;
  cout << "total_ore: " << 1000000000000 << endl;
}

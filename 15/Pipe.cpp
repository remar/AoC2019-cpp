#include "Pipe.h"

#include <iostream>

bool Pipe::input_ready() {
  if(instrumentation) {
    return true;
  }
  return !buffer.empty();
}

int Pipe::buffer_length() {
  return buffer.size();
}

long long int Pipe::read() {
  if(instrumentation) {
    return value;
  }

  if(buffer.empty()) {
    cout << name << " BUFFER EMPTY!" << endl;
    throw 0;
  }
  long long int val = buffer.front();
  buffer.pop_front();
  return val;
}

void Pipe::write(long long int val) {
  buffer.push_back(val);
}

void Pipe::clear() {
  buffer = {};
}

void Pipe::print() {
  cout << "[" << name << "] ";
  if(buffer.empty()) {
    cout << "empty";
  } else {
    for(auto x : buffer) {
      cout << x << " ";
    }
  }
  cout << endl;
}

void Pipe::set_instrumentation(long long int value) {
  this->value = value;
  instrumentation = true;
}

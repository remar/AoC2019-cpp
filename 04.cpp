#include <iostream>

using namespace std;

constexpr int len = 6;

bool has_pair(int (&code)[len]) {
  for(int i = 0;i < len - 1;i++) {
    if(code[i] == code[i+1]) {
      return true;
    }
  }
  return false;
}

bool has_pure_pair(int (&code)[len]) {
  int current_match = -1;
  int in_a_row = 0;
  for(int i {0};i < len;i++) {
    if(code[i] != current_match) {
      if(in_a_row == 2) {
	return true;
      }
      current_match = code[i];
      in_a_row = 1;
    } else {
      in_a_row++;
    }
  }
  return in_a_row == 2;
}

bool no_decreasing(int (&code)[len]) {
  for(int i = 0;i < len - 1;i++) {
    if(code[i] > code[i+1]) {
      return false;
    }
  }
  return true;
}

bool valid(int (&code)[len]) {
  return no_decreasing(code) && has_pure_pair(code);
}

bool equal(int (&code)[len], int (&end_code)[len]) {
  for(int i = 0;i < len;i++) {
    if(code[i] != end_code[i]) {
      return false;
    }
  }
  return true;
}

void increase(int (&code)[len]) {
  for(int i = len - 1;i >= 0;i--) {
    code[i] = (code[i] + 1)%10;
    if(code[i] != 0) {
      break;
    }
  }
}

void print(int (&code)[len]) {
  for(int i = 0;i < len;i++) {
    cout << code[i];
  }
  cout << endl;
}

int main() {
  int code[len] = {1,8,3,5,6,4};
  int end_code[len] = {6,5,7,4,7,4};

  int valid_codes = 0;

  while(!equal(code, end_code)) {
    if(valid(code)) {
      valid_codes++;
    }
    increase(code);
  }

  cout << "valid codes: " << valid_codes << endl;

  return 0;
}

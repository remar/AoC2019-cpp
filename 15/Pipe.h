#include <string>
#include <deque>

using namespace std;

class Pipe {
public:
  string name;
  bool input_ready();
  int buffer_length();
  long long int read();
  void write(long long int val);
  void clear();
  void print();
  void set_instrumentation(long long int value);

private:
  deque<long long int> buffer;
  bool instrumentation = false;
  long long int value;
};

#include <any>
#include <iostream>
#include <typeinfo>
#include <unordered_map>
#define cast(X) (*any_cast<unordered_map<string, any> *>(X))

using namespace std;

class objeto {
 public:
  unordered_map<string, any> GLOBAL;
};

unordered_map<string, any> deCast(any &x) {
  return (*any_cast<unordered_map<string, any> *>(x));
}

int main() {
  any GLOBAL = new unordered_map<string, any>();

  cast(GLOBAL)["a"] = 3;

  cast(GLOBAL)["b"] = new unordered_map<string, any>();

  string entry = "abc";
  cast(cast(GLOBAL)["b"])["c"] = entry;

  cout << any_cast<int> (deCast(GLOBAL)["a"]) << endl;
  cout << any_cast<string> (cast(cast(GLOBAL)["b"])["c"]) << endl;

  return 0;
}
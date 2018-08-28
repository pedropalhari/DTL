#include <any>
#include <iostream>
#include <typeinfo>
#include <unordered_map>
#define cast(X) (*any_cast<unordered_map<string, any> *>(X))

#define cast2(X) (*any_cast<unordered_map<string, basicObject> *>(X.obj))

using namespace std;

class objeto {
 public:
  unordered_map<string, any> GLOBAL;
};

typedef struct {
  int type;
  any obj;
} basicObject;

void decast(basicObject x) {
  if (x.type == 1) cout << any_cast<int>(x.obj) << endl;
}

int main() {
  any GLOBAL = new unordered_map<string, any>();
  cast(GLOBAL)["a"] = 3;
  cast(GLOBAL)["b"] = new unordered_map<string, any>();
  string entry = "abc";
  cast(cast(GLOBAL)["b"])["c"] = entry;
  cout << any_cast<int>(cast(GLOBAL)["a"]) << endl;
  cout << any_cast<string>(cast(cast(GLOBAL)["b"])["c"]) << endl;

  basicObject GLOBAL2;
  GLOBAL2.type = 0;
  GLOBAL2.obj = new unordered_map<string, basicObject>();

  basicObject obj1;
  obj1.type = 1;
  obj1.obj = 3;

  cast2(GLOBAL2)["a"] = obj1;

  decast(cast2(GLOBAL2)["a"]);

  basicObject obj2;
  obj2.type = 0;
  obj2.obj = new unordered_map<string, basicObject>();

  basicObject obj3;
  obj3.type = 1;
  obj3.obj = 4;

  cast2(GLOBAL2)["b"] = obj2;

  cast2(cast2(GLOBAL2)["b"])["c"] = obj3;

  decast(cast2(cast2(GLOBAL2)["b"])["c"]);

  return 0;
}
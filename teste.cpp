#include <any>
#include <iostream>
#include <typeinfo>
#include <unordered_map>
#define cast(X) (*any_cast<unordered_map<string, basicObject> *>(X.obj))

using namespace std;

typedef struct {
  int type;
  any obj;
} basicObject;

void decast(basicObject x) {
  if (x.type == 1) cout << any_cast<int>(x.obj) << endl;
}

int main() {
  basicObject GLOBAL;
  GLOBAL.type = 0;
  GLOBAL.obj = new unordered_map<string, basicObject>();

  basicObject obj1;
  obj1.type = 1;
  obj1.obj = 3;

  cast(GLOBAL)["a"] = obj1;

  basicObject obj2;
  obj2.type = 0;
  obj2.obj = new unordered_map<string, basicObject>();

  basicObject obj3;
  obj3.type = 1;
  obj3.obj = 4;

  cast(GLOBAL)["b"] = obj2;

  cast(cast(GLOBAL)["b"])["c"] = obj3;

  decast(cast(GLOBAL)["a"]);
  decast(cast(cast(GLOBAL)["b"])["c"]);

  return 0;
}
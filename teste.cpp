#include <any>
#include <functional>
#include <iostream>
#include <typeinfo>
#include <unordered_map>
#include <vector>
#define cast(X) (*any_cast<unordered_map<string, basicObject> *>(X.obj))

using namespace std;

enum Type { Object, Integer, Float, String };

vector<function<void()>> runProgram;

typedef struct {
  Type type;
  any obj;
} basicObject;

void decast(basicObject x) {
  if (x.type == Integer) cout << any_cast<int>(x.obj) << endl;
}

basicObject GLOBAL;

void add(basicObject &x, basicObject value) { x = value; }

int main() {
  GLOBAL.type = Object;
  GLOBAL.obj = new unordered_map<string, basicObject>();

  string anyTypeString = "NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE";
  string text = "as";
  any i = text;
  cout << "COMPARE " << i.type().name() << endl;
  if ((anyTypeString.compare(i.type().name()) == 0)){
    cout << "ENTROU NO IF" << endl;
    cout << any_cast<string>(i) << ", ";
  }
  else
    cout << any_cast<int>(i) << ", ";

  basicObject obj1;
  obj1.type = Integer;
  obj1.obj = 3;

  runProgram.push_back([obj1]() { add(cast(GLOBAL)["aa"], obj1); });

  for (auto i : runProgram) i();

  decast(cast(GLOBAL)["aa"]);

  // a = 3;
  cast(GLOBAL)["a"] = obj1;

  basicObject obj2;
  obj2.type = Object;
  obj2.obj = new unordered_map<string, basicObject>();

  basicObject obj3;
  obj3.type = Integer;
  obj3.obj = 4;

  // b = {};
  // b.c = 4;
  cast(GLOBAL)["b"] = obj2;
  cast(cast(GLOBAL)["b"])["c"] = obj3;

  decast(cast(GLOBAL)["a"]);
  decast(cast(cast(GLOBAL)["b"])["c"]);

  return 0;
}
#include <any>
#include <iostream>
#include <typeinfo>
#include <unordered_map>
#include <vector>
#include <functional>
#define cast(X) (*any_cast<unordered_map<string, basicObject> *>(X.obj))

using namespace std;

enum Type
{
  Object,
  Integer,
  Float,
  String
};

vector<function<void()>> runProgram;

typedef struct
{
  Type type;
  any obj;
} basicObject;

void decast(basicObject x)
{
  if (x.type == Integer)
    cout << any_cast<int>(x.obj) << endl;
}

basicObject GLOBAL;

void add(string x, int a, int b)
{
  basicObject aux;
  aux.obj = a + b;
  aux.type = Integer;
  cast(GLOBAL)[x] = aux;
}

int main()
{
  GLOBAL.type = Object;
  GLOBAL.obj = new unordered_map<string, basicObject>();

  runProgram.push_back([]() { add("teste", 2, 3); });

  for(auto i : runProgram)
    i();

  decast(cast(GLOBAL)["teste"]);

  basicObject obj1;
  obj1.type = Integer;
  obj1.obj = 3;

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
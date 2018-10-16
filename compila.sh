bison -d dtl.y &&
flex dtl.l &&
g++ dtl.tab.c lex.yy.c -lfl -o dtlex -std=c++17 &&
./dtlex entrada.dtl 
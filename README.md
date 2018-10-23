## Variáveis:

* Structs do tipo:
```Cpp
typedef struct {
  Type type;
  any obj;
} basicObject;
```

* Um objeto é um map para referenciar a.b.c do tipo: 
*any_cast<unordered_map<string, basicObject>*>

* Todos os objetos são globais.

* Objetos em cascata são adicionados em um array com cada nível sendo um elemento do array em tempo de parse (a.b.c => [a,b,c]), esse array é copiado para dentro da função lambda como um array local para de-referenciamento dentro da função posteriormente e o array é limpo para gerar o próximo;

## Instruções

* Todas as instruções são funções lambda com variáveis locais passadas para elas em tempo de Parsing (array de objetos em cascata para dereferenciar, nome das variáveis para dereferenciar, etc..).

* As funções lambda são _pushed_ em um array e em tempo de execução esse array é caminhado com um interator. Esse array faz somas, subtrações, comparações, pulos e avanços (estruturas de controle).

  
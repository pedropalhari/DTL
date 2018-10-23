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

## Instruções de controle

* As instruções de controle tem pilhas de cabeça e pé. Por exemplo:
* * Ifs tem cabeça e pé, se a condição é respeitada executa a instrução seguida da cabeça, se não, pula para a instrução seguinte ao pé.

* * Whiles tem a cabeça antes da condição, e uma instrução inserida antes do pé que faz retornar a cabeça. Se a condição é respeitada executa o corpo do while, se não pula o corpo inteiro (inclusive a que faz retornar a cabeça).

* * Funções tem uma instrução em sua cabeça que faz pular o corpo inteiro em sua declaração e uma instrução antes do seu pé que faz retornar aonde elas foram chamadas. Assim as instruções na hora da declaração não são executadas, e a chamada de função '()' coloca o iterator que executa as instruções dentro do corpo da função.

Para detectar cabeças/pés e lógicas de controle são usados diversos métodos em conjunto com o parser (Ex: whileAtomic ('while') -> whileHead (whileAtomic '(' express ')' ) -> whileBody (whileHead body)).

## Expressões

* Expressões tem de ser obviamente resolvidas em tempo de execução pois podem considerar variáveis, então são colocadas também no array de execução.

* Expressões utilizam uma stack para serem resolvidas.

* Caso a stack possua string é uma variável e tem de pegar o valor dela antes de continuar a execução da expressão.

* Expressões são todas as aritméticas e lógicas.

  
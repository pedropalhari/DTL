## Compilação

A compilação pode ser feita utilizando o arquivo `compila.sh` que compila os arquivos dtl.l, dtl.y e gera no final o arquivo dtlex com o g++ e executa o arquivo entrada.dtl, ou também pode ser obtido com o comando:
```
bison -d dtl.y && \
flex dtl.l && \
g++ dtl.tab.c lex.yy.c -lfl -o dtlex -std=c++17 && \
./dtlex entrada.dtl 
```

## Execução

Para executar um arquivo DTL basta invocar o parser como `./dtlex <nomeDoArquivo>.dtl`

O compilador tem 3 modos definidos por dois `#define` no inicio do dtl.y.
* **DEBUG_MODE**
  * TRUE - DEBUG em modo de execução
  * FALSE - Sem DEBUG em modo de execução
* **DEBUG_FIND_TOKEN**
  * TRUE - DEBUG dos tokens encontrados em parsing.
  * FALSE - Sem DEBUG dos tokens encontrados em parsing.

Na pasta `compilado/` é possível encontrar os arquivos dtlex (compilador normal), dtlexDEBUG (compilador com DEBUG_FIND_TOKEN=TRUE) e os arquivos DTL fatorial.dtl (programa fatorial na linguagem DTL), exemploAninhado (exemplo de dois whiles aninhados simulando caminhamento em matriz de 2 fors) e senha.dtl (exemplo do jogo A SENHA em .dtl).
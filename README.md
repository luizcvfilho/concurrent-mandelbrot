# Projeto Final - Cálculo do Conjunto de Mandelbrot

Este repositório contém dois arquivo `.c` desenvolvidos para o projeto final da disciplina de Programação Concorrente. Onde realizaremos o cálculo do conjunto de Mandelbrot para um intervalo específico e gerarmos uma imagem no formato ppm representando o resultado. Para visualizar mais detalhes na imagem gerada, pode-se aumentar a resolução da mesma alterando os valores definidos no programa.

## Arquivos

1. **sequential-version.c**:
O programa então fará as iterações sobre cada pixel da imagem que será gerada, para calcular o valor do conjunto para aquele ponto específico do plano complexo. Será então gerada uma imagem em formato ppm mostrando o plano gerado.

2. **concurrent-version.c**:
Recebe um valor como argumento da execução, referente à quantidade de threads que serão usadas. O programa então fará as iterações sobre cada pixel da imagem que será gerada, para calcular o valor do conjunto para aquele ponto específico do plano complexo. O cálculo será feito dividindo a quantidade de pixels de maneira igualitária entre todas as threadhs que irão executar de maneira concorrente. Será então gerada uma imagem em formato ppm mostrando o plano gerado.

## Como Compilar

Para compilar os arquivo, utilize o `gcc` no terminal. Execute os seguintes comandos:

```bash
gcc -o concurrent-version.exe concurrent-version.c -Wall -lm
```

```bash
gcc -o sequential-version.exe sequential-version.c -Wall -lm
```

## Como Executar

Após a compilação, execute os programas com o comando abaixo:

### Executar `sequential-version.c`

```bash
./sequential-version
```

### Executar `concurrent-version.c`

T = número de threadhs a serem utilizadas

```bash
./concurrent-version T
```

## Conclusão

Pode-se observar que a versão concorrente, sobretudo com maior quantidade de threads, apresenta um desempenho muito superior á versão sequencial, tendo em vista que o trabalho que será feito em toda a matriz é dividido, facilitando a execução.

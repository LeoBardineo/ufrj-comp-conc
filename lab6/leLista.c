#include <stdio.h>
#include <stdlib.h>

int leLista(long long int N, char *arquivo, int **lista) {
  int *listaLocal;
  FILE *descritorArquivo;  // descritor do arquivo de entrada
  size_t ret;              // retorno da funcao de leitura no arquivo de entrada

  // abre o arquivo para leitura binaria
  descritorArquivo = fopen(arquivo, "rb");
  if (!descritorArquivo) {
    fprintf(stderr, "Erro de abertura do arquivo\n");
    return 2;
  }

  // aloca memoria para a lista
  listaLocal = (int *)malloc(sizeof(int) * N);
  if (!listaLocal) {
    fprintf(stderr, "Erro de alocao da memoria da lista\n");
    return 3;
  }

  // carrega a lista de elementos do tipo int do arquivo
  ret = fread(listaLocal, sizeof(int), N, descritorArquivo);
  if (ret < N) {
    fprintf(stderr, "Erro de leitura dos elementos da lista\n");
    return 4;
  }

  // finaliza o uso das variaveis
  fclose(descritorArquivo);
  free(listaLocal);
  return 0;
}

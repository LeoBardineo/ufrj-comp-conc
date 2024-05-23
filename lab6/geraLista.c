#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "primalidade.c"

// descomentar o define abaixo caso deseje imprimir uma versao truncada da lista gerada no formato texto
#define TEXTO
// número máximo que pode ser colocado na lista
#define NUMERO_MAX 1000

int main(int argc, char *argv[]) {
  int *lista;              // lista de números
  long long int N;         // quantidade de números
  FILE *descritorArquivo;  // descritor do arquivo de saida
  size_t ret;              // retorno da funcao de escrita no arquivo de saida

  // recebe os argumentos de entrada
  if (argc < 3) {
    fprintf(stderr, "Digite: %s <quantidade de numeros> <arquivo saida>\n", argv[0]);
    return 1;
  }
  N = atoi(argv[1]);

  // aloca memoria para a lista
  lista = (int *)malloc(sizeof(int) * N);
  if (!lista) {
    fprintf(stderr, "Erro de alocao da memoria da lista\n");
    return 2;
  }

  // preenche a lista com valores inteiros aleatorios
  // randomiza a sequencia de numeros aleatorios
  srand(time(NULL));
  for (long int i = 0; i < N; i++) {
    *(lista + i) = rand() % NUMERO_MAX;
  }

// imprimir na saida padrao a lista gerada
#ifdef TEXTO
  for (int i = 0; i < N; i++) {
    fprintf(stdout, "%d ", lista[i]);
  }
  fprintf(stdout, "\n");
#endif

  // escreve a lista no arquivo
  // abre o arquivo para escrita binaria
  descritorArquivo = fopen(argv[2], "wb");
  if (!descritorArquivo) {
    fprintf(stderr, "Erro de abertura do arquivo\n");
    return 3;
  }
  // escreve os elementos da lista
  ret = fwrite(lista, sizeof(int), N, descritorArquivo);
  if (ret < N) {
    fprintf(stderr, "Erro de escrita no arquivo\n");
    return 4;
  }

  // finaliza o uso das variaveis
  fclose(descritorArquivo);
  free(lista);
  return 0;
}

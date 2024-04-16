#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "funcoes.c"
#include "timer.h"

float *matA;   // matriz de entrada A
float *matB;   // matriz de entrada B
float *saida;  // matriz de saida

typedef struct {
  int linhaInicial;
  int linhaFinal;
  int dim;
} tArgs;

void *tarefa(void *arg) {
  tArgs *args = (tArgs *)arg;
  int linhaInicial = args->linhaInicial;
  int linhaFinal = args->linhaFinal;
  int dim = args->dim;

  for (int i = linhaInicial; i < linhaFinal; i++) {
    for (int j = 0; j < dim; j++) {
      for (int k = 0; k < dim; k++) {
        saida[i * dim + j] += matA[i * dim + k] * matB[k * dim + j];
      }
    }
  }
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  char *arquivoMatrizA, *arquivoMatrizB, *arquivoMatrizSaida;
  int dim, err, nThreads;
  double inicio, fim, delta;
  pthread_t *tid;
  tArgs *args;

  // leitura e avaliacao dos parametros de entrada
  GET_TIME(inicio);
  if (argc < 5) {
    fprintf(stderr, "Digite: %s <arquivo matriz A> <arquivo matriz B> <arquivo matriz saida> <numero de threads>\n", argv[0]);
    return 1;
  }
  arquivoMatrizA = argv[1];
  arquivoMatrizB = argv[2];
  arquivoMatrizSaida = argv[3];
  nThreads = atoi(argv[4]);

  err = leMatrizBinario(arquivoMatrizA, &matA, &dim);
  if (err) return err;

  err = leMatrizBinario(arquivoMatrizB, &matB, &dim);
  if (err) return err;

  // alocacao de memoria para as estruturas de dados
  err = aloca(dim, &saida);
  if (err) return err;

  // inicializacao da saida
  inicializa(dim, &saida);
  GET_TIME(fim);
  delta = fim - inicio;
  printf("Tempo inicializacao:%lf\n", delta);

  // multiplicacao das matrizes
  GET_TIME(inicio);
  tid = (pthread_t *)malloc(sizeof(pthread_t) * dim);
  if (tid == NULL) {
    puts("Erro Malloc");
    return 2;
  }

  args = (tArgs *)malloc(sizeof(tArgs) * dim);
  if (args == NULL) {
    puts("Erro Malloc");
    return 2;
  }

  int linhaInicial = 0;
  int linhasPorThread = dim / nThreads;
  for (int i = 0; i < nThreads; i++) {
    (args + i)->linhaInicial = linhaInicial;
    (args + i)->linhaFinal = linhaInicial + linhasPorThread;
    (args + i)->dim = dim;

    if (i == nThreads - 1) {
      (args + i)->linhaFinal = dim;
    }

    if (pthread_create(tid + i, NULL, tarefa, (void *)(args + i))) {
      puts("Erro pthread_create");
      return 3;
    }

    linhaInicial = (args + i)->linhaFinal;
  }

  for (int i = 0; i < nThreads; i++) {
    pthread_join(*(tid + i), NULL);
  }

  GET_TIME(fim);
  delta = fim - inicio;
  printf("Tempo multiplicacao (dimensao %d): %lf\n", dim, delta);

  // escrita em binario
  err = escreveMatrizBinario(arquivoMatrizSaida, saida, dim);
  if (err) return err;

  // liberacao da memoria
  GET_TIME(inicio);
  free(matA);
  free(matB);
  free(saida);
  free(args);
  free(tid);
  GET_TIME(fim)
  delta = fim - inicio;
  printf("Tempo finalizacao:%lf\n", delta);

  return 0;
}
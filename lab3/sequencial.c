#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "funcoes.c"
#include "timer.h"

float *matA;   // matriz de entrada A
float *matB;   // matriz de entrada B
float *saida;  // matriz de saida

int main(int argc, char *argv[]) {
  char *arquivoMatrizA, *arquivoMatrizB;
  int dim, err;
  double inicio, fim, delta;

  // leitura e avaliacao dos parametros de entrada
  GET_TIME(inicio);
  if (argc < 3) {
    fprintf(stderr, "Digite: %s <arquivo matriz A> <arquivo matriz B>\n", argv[0]);
    return 1;
  }
  arquivoMatrizA = argv[1];
  arquivoMatrizB = argv[2];

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
  multiplicaMatrizSequencial(dim, &matA, &matB, &saida);
  GET_TIME(fim);
  delta = fim - inicio;
  printf("Tempo multiplicacao (dimensao %d): %lf\n", dim, delta);

  // TODO: escrita em binario

  // liberacao da memoria
  GET_TIME(inicio);
  free(matA);
  free(matB);
  free(saida);
  GET_TIME(fim)
  delta = fim - inicio;
  printf("Tempo finalizacao:%lf\n", delta);

  return 0;
}
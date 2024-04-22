#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "timer.h"

long long int qtdPrimos = 0;
pthread_mutex_t mutex;

typedef struct {
  long long inicio;
  long long fim;
} t_args;

int ehPrimo(long long int n) {
  int i;
  if (n <= 1) return 0;
  if (n == 2) return 1;
  if (n % 2 == 0) return 0;
  for (i = 3; i < sqrt(n) + 1; i += 2)
    if (n % i == 0) return 0;
  return 1;
}

void *tarefa(void *arg) {
  t_args *args = (t_args *)arg;
  long long inicio = args->inicio;
  long long fim = args->fim;
  // Comentado, a solução sem possibilidade de corrida de dados
  // long long int qtdPrimosLocal = 0;

  for (long long i = inicio; i <= fim; i++) {
    // qtdPrimosLocal += ehPrimo(1);
    if (ehPrimo(i)) {
      pthread_mutex_lock(&mutex);
      qtdPrimos++;
      pthread_mutex_unlock(&mutex);
    }
  }

  // qtdPrimos += qtdPrimosLocal;

  pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {
  if (argc < 3) {
    printf("Use: %s <numero de threads> <numero de elementos> \n", argv[0]);
    return 1;
  }
  int nThreads = atoi(argv[1]);
  long long int N = atoll(argv[2]);
  ;

  pthread_t threads[nThreads];
  long long int intervalo = N / nThreads;
  long long int inicio = 1;
  long long int fim = intervalo;
  double inicioT, fimT, delta;

  t_args *args;
  args = (t_args *)malloc(sizeof(t_args) * nThreads);
  if (args == NULL) {
    puts("Erro Malloc");
    return 2;
  }

  // Criar threads
  GET_TIME(inicioT);
  pthread_mutex_init(&mutex, NULL);
  for (int i = 0; i < nThreads; i++) {
    args[i].inicio = inicio;
    args[i].fim = (i == nThreads - 1) ? N : fim;

    if (pthread_create(&threads[i], NULL, tarefa, (void *)(args + i)) != 0) {
      fprintf(stderr, "Erro pthread_create.\n");
      return 3;
    }

    inicio = fim + 1;
    fim += intervalo;
  }

  // Aguardar as threads terminarem
  for (int i = 0; i < nThreads; i++) {
    if (pthread_join(threads[i], NULL) != 0) {
      fprintf(stderr, "Erro pthread_join.\n");
      return 3;
    }
  }
  pthread_mutex_destroy(&mutex);
  GET_TIME(fimT);
  delta = fimT - inicioT;
  printf("Quantidade de primos: %lld \n", qtdPrimos);
  printf("Durou %.2f segundos\n", delta);

  return 0;
}
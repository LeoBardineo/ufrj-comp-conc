#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#include "primalidade.c"

#define DEBUG

typedef struct {
  int id;
} tArgs;

int *buffer, *listaQtdPrimos, tam;
sem_t mutex, listaVazia, listaCheia;

int M;

void *produtor(void *arg) {
  static int in = 0;
  const char *arquivo = (const char *)arg;
  FILE *descritorArquivo = fopen(arquivo, "rb");
  for (int i = 0; i < M; i++) {
    int num;
    fread(&num, sizeof(int), 1, descritorArquivo);

    sem_wait(&listaVazia);
    sem_wait(&mutex);

    buffer[in] = num;
    in = (in + 1) % M;
    tam++;

    sem_post(&mutex);
    sem_post(&listaCheia);
  }

  fclose(descritorArquivo);
  pthread_exit(NULL);
}

void *consumidor(void *arg) {
  tArgs *args = (tArgs *)arg;
  static int out = 0;
  int qtdPrimos = 0;

  while (1) {
    sem_wait(&listaCheia);
    sem_wait(&mutex);

    if (M == 0 && tam == 0) {
      sem_post(&mutex);
      sem_post(&listaCheia);
      listaQtdPrimos[args->id] = qtdPrimos;
      printf("id %d | %d primos\n", args->id, qtdPrimos);
      pthread_exit(NULL);
    }

    int num = buffer[out];
    out = (out + 1) % M;
    tam--;
    M--;

    if (ehPrimo(num))
      qtdPrimos++;

    sem_post(&mutex);
    sem_post(&listaVazia);
  }

  pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {
  int nThreads, err;
  long long int N;

  if (argc < 4) {
    fprintf(stderr, "Digite: %s <quantidade de threads> <tamanho do buffer> <arquivo entrada>\n", argv[0]);
    return 1;
  }

  nThreads = atoi(argv[1]);
  M = atoi(argv[2]);
  const char *arquivo = argv[3];

  listaQtdPrimos = (int *)malloc(sizeof(int) * nThreads);
  if (listaQtdPrimos == NULL) {
    puts("Erro Malloc");
    return 2;
  }

  buffer = (int *)malloc(sizeof(int) * M);
  if (buffer == NULL) {
    puts("Erro Malloc");
    return 2;
  }

  sem_init(&mutex, 0, 1);
  sem_init(&listaVazia, 0, M);
  sem_init(&listaCheia, 0, 0);

  pthread_t tProdutor, *tConsumidor;
  tConsumidor = (pthread_t *)malloc(sizeof(pthread_t) * M);
  if (tConsumidor == NULL) {
    puts("Erro Malloc");
    return 2;
  }

  tArgs *threadArgs;
  threadArgs = (tArgs *)malloc(sizeof(tArgs) * M);
  if (threadArgs == NULL) {
    puts("Erro Malloc");
    return 2;
  }

  pthread_create(&tProdutor, NULL, produtor, (void *)arquivo);

  for (int i = 0; i < M; i++) {
    threadArgs[i].id = i;
    pthread_create(&tConsumidor[i], NULL, consumidor, (void *)&threadArgs[i]);
  }

  pthread_join(tProdutor, NULL);

  int maiorQtdPrimos = 0, totalQtdPrimos = 0, idVencedor;
  for (int i = 0; i < M; i++) {
    totalQtdPrimos += listaQtdPrimos[i];
    if (listaQtdPrimos[i] > maiorQtdPrimos) {
      maiorQtdPrimos = listaQtdPrimos[i];
      idVencedor = i;
    }
  }

  printf("Total de primos: %d\n");
  printf("Consumidora de id %d venceu com %d primos encontrados.\n");

  sem_destroy(&mutex);
  sem_destroy(&listaVazia);
  sem_destroy(&listaCheia);
  free(buffer);
  free(threadArgs);
  free(tConsumidor);
  free(listaQtdPrimos);

  return 0;
}

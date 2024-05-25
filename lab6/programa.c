#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#include "primalidade.c"

#define DEBUG

typedef struct {
  int id;
  int qtdPrimos;
} tArgs;

int *buffer, *listaQtdPrimos, terminouLeitura, tamC, tamP, M;
sem_t mutex, listaVazia, listaCheia;

void *produtor(void *arg) {
  static int in = 0;
  const char *arquivo = (const char *)arg;
  FILE *descritorArquivo = fopen(arquivo, "rb");
  int num;
  while (fread(&num, sizeof(int), 1, descritorArquivo) == 1) {
    sem_wait(&listaVazia);
    sem_wait(&mutex);

    buffer[in] = num;
    in = (in + 1) % M;
    tamP++;

    sem_post(&mutex);
    sem_post(&listaCheia);
  }

  terminouLeitura = 1;
  fclose(descritorArquivo);
  sem_post(&mutex);
  sem_post(&listaCheia);
  pthread_exit(NULL);
}

void *consumidor(void *arg) {
  tArgs *args = (tArgs *)arg;
  static int out = 0;
  args->qtdPrimos = 0;

  while (1) {
    sem_wait(&listaCheia);
    sem_wait(&mutex);

    if (terminouLeitura && tamP == tamC) {
      sem_post(&mutex);
      sem_post(&listaCheia);
      listaQtdPrimos[args->id] = args->qtdPrimos;
      printf("%d  | %d\n", args->id, args->qtdPrimos);
      pthread_exit(NULL);
    }

    int num = buffer[out];
    out = (out + 1) % M;
    tamC++;

    if (ehPrimo(num))
      args->qtdPrimos++;

    sem_post(&mutex);
    sem_post(&listaVazia);
  }

  pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {
  int nThreads;

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

  printf("id | qtdPrimos\n");
  printf("---+----------\n");
  for (int i = 0; i < M; i++) {
    threadArgs[i].id = i;
    pthread_create(&tConsumidor[i], NULL, consumidor, (void *)&threadArgs[i]);
  }

  pthread_join(tProdutor, NULL);
  for (int i = 0; i < nThreads; i++) {
    if (pthread_join(tConsumidor[i], NULL) != 0) {
      fprintf(stderr, "Erro pthread_join.\n");
      return 3;
    }
  }

  int maiorQtdPrimos = -1, totalQtdPrimos = 0, idVencedor;
  for (int i = 0; i < nThreads; i++) {
    totalQtdPrimos += listaQtdPrimos[i];
    if (listaQtdPrimos[i] > maiorQtdPrimos) {
      maiorQtdPrimos = listaQtdPrimos[i];
      idVencedor = i;
    }
  }

  printf("Total de primos: %d\n", totalQtdPrimos);
  printf("Consumidora de id %d venceu com %d primos encontrados.\n", idVencedor, maiorQtdPrimos);

  sem_destroy(&mutex);
  sem_destroy(&listaVazia);
  sem_destroy(&listaCheia);
  free(buffer);
  free(threadArgs);
  free(tConsumidor);
  free(listaQtdPrimos);

  return 0;
}

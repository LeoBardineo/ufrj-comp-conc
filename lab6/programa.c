#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "primalidade.c"

#define DEBUG

typedef struct {
  int id;
  int primos;
} tArgs;

int *buffer, tam, totalPrimos, idVencedor;
sem_t mutex, listaVazia, listaCheia;

int M;
char *arquivo;

void *produtor(void *arg) {
  static int in = 0;
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
      args->primos = qtdPrimos;
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
  arquivo = argv[3];

  pthread_t t_produtor;

  return 0;
}

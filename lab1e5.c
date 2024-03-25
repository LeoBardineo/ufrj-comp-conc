#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int* array;
typedef struct {
    int initialIndex;
    int lastIndex;
} t_argsArray;

void calculaQuadrado(void *args) {
    // passar array por referência ou usar ele como variável global?
    t_argsArray argsArray = *(t_argsArray*) args;
}

int main(int argc, char* argv[]) {    
    if(argc < 3) { 
        printf("ERRO 400: informe a quantidade de threads <%s> <nthreads>\n", argv[0]);
        exit(-1);
    }

    pthread_t tid_sistema[M];
    t_argsArray *argsArray;
    int M, N;
    M = atoi(argv[1]);
    N = atoi(argv[2]);
    if(N > M) {
        M = N;
    }

    argsArray = malloc(sizeof(t_argsArray));
    if (argsArray == NULL) {
      printf("ERRO 500: erro ao alocar memória para o argsArray\n");
      exit(-2);
    }

    array = malloc(N * sizeof(int));
    if (array == NULL) {
      printf("ERRO 500: erro ao alocar memória para o array\n");
      exit(-2);
    }

    /*
        [0, 1]
        [2, 3]
        [4, 5]
        [6, 7]
        [8, 9]
    */

    if(N % M == 0) {
        int passo = N/M;
        for(int i = 0; i < M; i += passo) {
            argsArray->initialIndex = i;
            argsArray->lastIndex = i + passo - 1;
            if (pthread_create(&tid_sistema[i], NULL, calculaQuadrado, (void*) argsArray)) {
                printf("ERRO 500: erro ao criar a thread\n");
                exit(-2);
            }
        }
    }

    /*
    M = 5, N = 10
    cada thread pega 2 elementos, N/M

    M = 3, N = 10
    [3, 3, 4]
    se N % M != 0, uma thread com N % M, e o resto com N / M

    M = 10, N = 5
    se N > M, M = N, cada thread com um elemento

    */

    for (int i = 0; i < M; i++) {
        if (pthread_join(tid_sistema[i], NULL)) {
            printf("ERRO 500: erro ao dar pthread_join()\n");
            exit(-1);
        }
    }

    pthread_exit(NULL);
}
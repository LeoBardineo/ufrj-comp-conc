#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Para executar:
// colocar os números do array no input.txt
// ./a <nthreads> <tamanho> < input.txt

int* array;
int* arrayTeste;
typedef struct {
    int initialIndex;
    int lastIndex;
} t_argsArray;

void *calculaQuadrado(void *args) {
    t_argsArray argsArray = *(t_argsArray*) args;
    for(int i = argsArray.initialIndex; i <= argsArray.lastIndex; i++){
        array[i] *= array[i];
    }
    pthread_exit(NULL);
}

void printaArray(int N) {
    printf("[");
    for (int i = 0; i < N-1; i++) {
        printf("%d, ", array[i]);
    }
    printf("%d]", array[N-1]);
}

int main(int argc, char* argv[]) {
    puts("Programa concorrente para elevar ao quadrado todos os elementos de um array");
    if(argc < 3) { 
        printf("ERRO 400: informe a quantidade de threads e tamanho do array <%s> <nthreads> <tamanho>\n", argv[0]);
        exit(-1);
    }

    int M, N, mThreadsCriadas = 0;
    M = atoi(argv[1]);
    N = atoi(argv[2]);
    if(M > N) {
        M = N;
    }

    pthread_t tid_sistema[M];

    array = malloc(N * sizeof(int));
    if (array == NULL) {
      printf("ERRO 500: erro ao alocar memória para o array\n");
      exit(-2);
    }

    arrayTeste = malloc(N * sizeof(int));
    if (arrayTeste == NULL) {
      printf("ERRO 500: erro ao alocar memória para o arrayTeste\n");
      exit(-2);
    }

    for(int i = 0; i < N; i++) {
        printf("Entre com o %dº número: ", i+1);
        scanf("%d", &(array[i]));
        arrayTeste[i] = array[i];
    }
    puts("\n");
    printf("Array inicial:\n");
    printaArray(N);

    int elementosExtras = N % M;
    int passo = N / M;
    puts("");
    for(int i = 0; i < N; i += passo) {
        t_argsArray *argsArray = malloc(sizeof(t_argsArray));
        if (argsArray == NULL) {
            printf("ERRO 500: erro ao alocar memória para o argsArray\n");
            exit(-2);
        }

        argsArray->initialIndex = i;
        argsArray->lastIndex = i + passo + elementosExtras - 1;
        if (pthread_create(&tid_sistema[mThreadsCriadas], NULL, calculaQuadrado, (void*) argsArray)) {
            printf("ERRO 500: erro ao criar a thread\n");
            exit(-2);
        }

        if(elementosExtras > 0){
            i += elementosExtras;
            elementosExtras -= elementosExtras;
        }
        
        mThreadsCriadas++;
    }

    for (int i = 0; i < mThreadsCriadas; i++) {
        if (pthread_join(tid_sistema[i], NULL)) {
            printf("\nERRO 500: erro ao dar pthread_join()\n");
            exit(-1);
        }
    }

    puts("");
    printf("Array final:\n");
    printaArray(N);
    
    puts("\n");
    printf("Teste:\n");
    int testePassou = 1;
    for(int i = 0; i < N; i++){
        printf("%d * %d == %d \t| ", arrayTeste[i], arrayTeste[i], array[i]);
        if(arrayTeste[i] * arrayTeste[i] == array[i]) {
            printf("CORRETO\n");
        } else {
            printf("FALSO\n");
            testePassou = 0;
        }
    }
    if(testePassou) {
        printf("O teste passou");
    } else {
        printf("O teste falhou");
    }

    pthread_exit(NULL);
}
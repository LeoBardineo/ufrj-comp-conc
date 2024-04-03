#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define TESTE

float *vetor; //vetor de entrada

typedef struct {
   int id; //id da thread
   long int tamBloco; //tamanho do bloco (cada thread processa um bloco)
} tArgs;

typedef struct {
   float soma; // soma parcial, de uma thread
} tSoma;


void * tarefa(void * arg) {
   tArgs *args = (tArgs *) arg; 

   tSoma *somaParcial; // soma da thread
   somaParcial = (tSoma*) malloc(sizeof(tSoma));
   if(somaParcial==NULL) {
      pthread_exit(NULL);
   }
   
   long int ini = args->id * args->tamBloco; //elemento inicial do bloco da thread
   long int fim = ini + args->tamBloco; //elemento final(nao processado) do bloco da thread
   
   float soma = 0;
   for(long int i = ini; i < fim; i++)
      soma += vetor[i];
   somaParcial->soma = soma;
   
   // retorna o resultado da soma local
   pthread_exit((void *) somaParcial); 
}

int main(int argc, char *argv[]) {
   long int dim; //dimensao do vetor de entrada
   long int tamBloco; //tamanho do bloco de cada thread 
   int nthreads; //numero de threads que serao criadas
   float somaTotal = 0;
   long int excedentes;

   pthread_t *tid; //vetor de identificadores das threads no sistema
   tSoma *retorno; //valor de retorno das threads

#ifdef TESTE
   float somaCorreta; //valores de referencia para os testes
#endif

   if(argc < 2) {
       fprintf(stderr, "Digite: %s <numero threads>\n", argv[0]);
       return 1; 
   }
   nthreads = atoi(argv[1]);
   printf("nthreads=%d\n", nthreads); 

   //carrega/aloca o vetor de entrada
   scanf("%ld", &dim);
   printf("dim=%ld\n", dim);
   vetor = (float*) malloc(sizeof(float)*dim);
   if(vetor == NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }
   //preenche o vetor de entrada
   for(long int i=0; i<dim; i++)
      scanf("%f", &vetor[i]);

#ifdef TESTE
   for(long int i=0; i<dim; i++){
      printf("%f ", vetor[i]);
      somaCorreta += vetor[i];
   }
#endif

   tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
   if(tid==NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 3;
   }

   tamBloco = dim / nthreads;
   excedentes = dim % nthreads;
   for(int i=0; i < nthreads; i++) {
       tArgs *args = (tArgs*) malloc(sizeof(tArgs));
       if (args == NULL) {
          printf("--ERRO: malloc()\n"); 
          pthread_exit(NULL);
       }
       args->id = i; 
       args->tamBloco = tamBloco; 
       if(excedentes) {
         args->tamBloco += (i == nthreads-1) ? excedentes : 1;
         excedentes -= 1;
      }
       if(pthread_create(tid+i, NULL, tarefa, (void*) args)){
          fprintf(stderr, "ERRO--pthread_create\n");
          return 5;
       }
   }

   for(int i=0; i<nthreads; i++) {
      if(pthread_join(*(tid+i), (void**) &retorno)){
         fprintf(stderr, "ERRO--pthread_create\n");
         return 6;
      }
      somaTotal += retorno->soma;
   }

   printf("\n\nSoma total (concorrente): %f\n", somaTotal);

#ifdef TESTE
   printf("\n\nSoma correta (sequencial): %f\n", somaCorreta);
#endif

   free(vetor);
   free(tid);
   free(retorno);

   return 0;
}
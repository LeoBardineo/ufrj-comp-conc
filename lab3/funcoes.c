#include <stdio.h>
#include <stdlib.h>

int aloca(int dim, float **saida) {
  *saida = (float *)malloc(sizeof(float) * dim * dim);
  if (*saida == NULL) {
    printf("ERRO--malloc\n");
    return 2;
  }

  return 0;
}

void inicializa(int dim, float **saida) {
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      (*saida)[i * dim + j] = 0;
    }
  }
}

void printaMatriz(int dim, float *mat) {
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      printf("%f ", mat[i * dim + j]);  // mat[i][j]
    }
    printf("\n");
  }
  printf("\n");
}

void multiplicaMatrizSequencial(int dim, float **matA, float **matB, float **saida) {
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      for (int k = 0; k < dim; k++) {
        (*saida)[i * dim + j] += (*matA)[i * dim + k] * (*matB)[k * dim + j];
      }
    }
  }
}

int leMatrizBinario(char *arquivo, float **matriz, int *dim) {
  FILE *descritorArquivo;
  size_t ret;
  long long int tam;
  int linhas, colunas;

  // abre o arquivo para leitura binaria
  descritorArquivo = fopen(arquivo, "rb");
  if (!descritorArquivo) {
    fprintf(stderr, "Erro de abertura do arquivo\n");
    return 2;
  }

  // le as dimensoes da matriz
  ret = fread(&linhas, sizeof(int), 1, descritorArquivo);
  if (!ret) {
    fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
    return 3;
  }
  ret = fread(&colunas, sizeof(int), 1, descritorArquivo);
  if (!ret) {
    fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
    return 3;
  }
  tam = linhas * colunas;  // calcula a qtde de elementos da matriz

  // confere se matriz é quadrada
  if (linhas != colunas) {
    fprintf(stderr, "Erro: matriz não é quadrada");
    return 1;
  }
  *dim = linhas;

  // aloca memoria para a matriz
  *matriz = (float *)malloc(sizeof(float) * tam);
  if (!*matriz) {
    fprintf(stderr, "Erro de alocao da memoria da matriz\n");
    return 3;
  }

  // carrega a matriz de elementos do tipo float do arquivo
  ret = fread(*matriz, sizeof(float), tam, descritorArquivo);
  if (ret < tam) {
    fprintf(stderr, "Erro de leitura dos elementos da matriz\n");
    return 4;
  }

  fclose(descritorArquivo);

  return 0;
}

int escreveMatrizBinario(char *arquivoSaida, float *matriz, int dim) {
  FILE *descritorArquivo;
  size_t ret;
  int linhas = dim, colunas = dim;
  int tam = dim * dim;

  descritorArquivo = fopen(arquivoSaida, "wb");
  if (!descritorArquivo) {
    fprintf(stderr, "Erro de abertura do arquivo\n");
    return 3;
  }

  // escreve numero de linhas e de colunas
  ret = fwrite(&linhas, sizeof(int), 1, descritorArquivo);
  ret = fwrite(&colunas, sizeof(int), 1, descritorArquivo);
  // escreve os elementos da matriz
  ret = fwrite(matriz, sizeof(float), tam, descritorArquivo);
  if (ret < tam) {
    fprintf(stderr, "Erro de escrita no  arquivo\n");
    return 4;
  }
  fclose(descritorArquivo);

  return 0;
}
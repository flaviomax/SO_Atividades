/* Usa 3 tipos de thread para analisar cada linha, coluna e 
quadrado 3x3 de um sudoku. No total, são instanciadas 27 threads. */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

volatile int tabuleiro[9][9];
volatile int tabuleiro_aux[9][9][9];
/* No inicio, todos os valores sao possíveis; conforme os numeros vao sendo
catalogados em cada casa, as possibilidades sao diminuídas */
volatile int flag_errado = 0;

/** Thread que verifica se a linha (passada em v) é valida */
void* f_thread_linhas(void *v) {
  int m = *(int *) v;
  int k, n, boolean_ocorrencias[9] = {0}; 
  for (n = 0; n < 9; n++){
    if (tabuleiro [m][n] != 0){
      if (boolean_ocorrencias [tabuleiro [m][n] - 1] == 0){
        boolean_ocorrencias [tabuleiro [m][n] - 1] = 1;
      }    
      else{
        flag_errado = 1; /* Já há uma ocorrencia do numero na linha */
        printf("Mais de uma ocorrencia do numero %d na linha %d\n", tabuleiro [m][n], (m + 1));
        exit (1);
      }
    }
  }
  for (n = 0; n < 9; n++){
    if (tabuleiro [m][n] == 0)
      for (k = 0; k < 9; k++)
        if (boolean_ocorrencias[k] == 1)
          tabuleiro_aux [m][n][k] = 0;
  }
  return NULL;
}

/** Verifica a coluna v */
void* f_thread_colunas(void *v) {
  int m = *(int *) v;
  int n, boolean_ocorrencias[9] = {0};
  for (n = 0; n < 9; n++){
    if (tabuleiro [n][m] != 0){
      if (boolean_ocorrencias [tabuleiro [n][m] - 1] == 0){
        tabuleiro_aux [n][m][tabuleiro [n][m] - 1] = 0;
        boolean_ocorrencias [tabuleiro [n][m] - 1] = 1;
      }
      else{
        flag_errado = 1;
        printf("Mais de uma ocorrencia do numero %d na coluna %d\n", tabuleiro [n][m], (m + 1));
        exit (1);
      }
    }
  }
  return NULL;
}

/** Verifica se está válido o quadrado v 3x3. Os quadrados são contados
da esquerda para a direita, de cima para baixo */
void* f_thread_quadrados(void *v) {
  int quadrado = *(int *) v;
  int i, j, x, y, boolean_ocorrencias[9] = {0};
  x = (quadrado / 3) * 3; /* temos a linha do quadrado */
  y = (quadrado % 3) * 3; /* temos a coluna */
	for (i = x; i < (3 + x); i++){
		for (j = y; j < (3 + y); j++){
		  if (tabuleiro [i][j] != 0){
		    if (boolean_ocorrencias [tabuleiro [i][j] - 1] == 0){
		      tabuleiro_aux [i][j][tabuleiro [i][j] - 1] = 0;
		      boolean_ocorrencias [tabuleiro [i][j] - 1] = 1;
		    }
		    else{
		      flag_errado = 1;
				  printf("Mais de uma ocorrencia do numero %d no quadrado %d\n", tabuleiro [i][j], (quadrado + 1));
				  exit(1);
		    }
		  }
		}
	}
  return NULL;
}

int main() {

/* Leitura do tabuleiro */
  int i, j, k;
  for (i = 0; i < 9; i++)
    for (j = 0; j < 9; j++){
      char temp;
      scanf("%c ", &temp);
      if (temp != 'X')
				tabuleiro [i][j] = (temp - '0');
      else
        tabuleiro [i][j] = 0; /* substitui os X por 0, para poder guardar na matriz de inteiros */
    }
  for (i = 0; i < 9; i++)
    for (j = 0; j < 9; j++)
      for (k = 0; k < 9; k++)
        tabuleiro_aux[i][j][k] = 1;

/* Fim da leitura */

  pthread_t linhas[9], colunas[9], quadrados[9];
  
  for (i = 0; i < 9; i++){
		if (pthread_create(&linhas[i], NULL, f_thread_linhas,(void*) &i))
		  fprintf(stderr, "Erro na criacao da thread. \n");
	/*	if (pthread_create(&colunas[i], NULL, f_thread_colunas, (void*) &i))
	      fprintf(stderr, "Erro na criacao da thread. \n");
  	if (pthread_create(&quadrados[i], NULL, f_thread_quadrados, (void*) &i))
	      fprintf(stderr, "Erro na criacao da thread. \n"); */
	}
	
  for (i = 0; i < 9; i++){
  	pthread_join(linhas[i], NULL);
	//	pthread_join(colunas[i], NULL);
	//	pthread_join(quadrados[i], NULL);
  }
  /* Impressao */
  for (i = 0; i < 9; i++){
    for (j = 0; j < 9; j++){
      if (tabuleiro [i][j] != 0)
        printf ("%d ", tabuleiro [i][j]);
      else{
        printf ("(");
        for (k = 0; k < 9; k++){
          if (tabuleiro_aux[i][j][k] == 1)
            printf ("%d", (k + 1));
        }
        printf (") ");
      }
    }
    printf("\n");
  }

  return 0;
}

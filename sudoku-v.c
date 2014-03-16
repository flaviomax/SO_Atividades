/* Usa 3 tipos de thread para analisar cada linha, coluna e 
quadrado 3x3 de um sudoku. No total, são instanciadas 27 threads. */

#include <pthread.h>
#include <stdio.h>

volatile int tabuleiro[9][9];
volatile int flag_errado = 0;

/** Thread que verifica se a linha (passada em v) é valida */
void* f_thread_linhas_v(void *v) {
  int m = *(int *) v;
  int n, boolean_ocorrencias[9] = {0}; 
  for (n = 0; n < 9; n++){
    if (boolean_ocorrencias [tabuleiro [m][n] - 1] == 0)
      boolean_ocorrencias [tabuleiro [m][n] - 1] = 1;
    else{
      flag_errado = 1; /* Já há uma ocorrencia do numero na linha */
      printf("Mais de uma ocorrencia do numero %d na linha %d\n", tabuleiro [m][n], (m + 1));
    }
  }
  return NULL;
}

/** Verifica a coluna v */
void* f_thread_colunas_v(void *v) {
  int m = *(int *) v;
  int n, boolean_ocorrencias[9] = {0};
  for (n = 0; n < 9; n++){
    if (boolean_ocorrencias [tabuleiro [n][m] - 1] == 0)
      boolean_ocorrencias [tabuleiro [n][m] - 1] = 1;
    else{
      flag_errado = 1;
      printf("Mais de uma ocorrencia do numero %d na coluna %d\n", tabuleiro [n][m], (m + 1));
    }
  }
  return NULL;
}

/** Verifica se está válido o quadrado v 3x3. Os quadrados são contados
da esquerda para a direita, de cima para baixo */
void* f_thread_quadrados_v(void *v) {
  int quadrado = *(int *) v;
  int i, j, x, y, boolean_ocorrencias[9] = {0};
  x = (quadrado / 3) * 3; /* temos a linha do quadrado */
  y = (quadrado % 3) * 3; /* temos a coluna */
	for (i = x; i < (3 + x); i++){
		for (j = y; j < (3 + y); j++){
		 // printf ("entrou, quadrado %d, %d, %d\n", quadrado, i, j);
		  if (boolean_ocorrencias [tabuleiro [i][j] - 1] == 0)
		    boolean_ocorrencias [tabuleiro [i][j] - 1] = 1;
		  else{
		    flag_errado = 1;
				printf("Mais de uma ocorrencia do numero %d no quadrado %d\n", tabuleiro [i][j], (quadrado + 1));
		  }
		}
	}
  return NULL;
}

int main() {

/* Leitura do tabuleiro */
  int i, j;
  for (i = 0; i < 9; i++)
    for (j = 0; j < 9; j++){
      char temp;
      scanf("%c ", &temp);
      if (temp != 'X')
				tabuleiro [i][j] = (temp - '0');
      else
        tabuleiro [i][j] = 0; /* substitui os X por 0, para poder guardar na matriz de inteiros */
    }

/* Fim da leitura */

  pthread_t linhas_v[9], colunas_v[9], quadrados_v[9];
  
  for (i = 0; i < 9; i++){
		if (pthread_create(&linhas_v[i], NULL, f_thread_linhas_v,(void*) &i))
		  fprintf(stderr, "Erro na criacao da thread. \n");
		if (pthread_create(&colunas_v[i], NULL, f_thread_colunas_v, (void*) &i))
	      	  fprintf(stderr, "Erro na criacao da thread. \n");
  	    	if (pthread_create(&quadrados_v[i], NULL, f_thread_quadrados_v, (void*) &i))
	      	  fprintf(stderr, "Erro na criacao da thread. \n");
  }
	
  for (i = 0; i < 9; i++){
  	pthread_join(linhas_v[i], NULL);
	pthread_join(colunas_v[i], NULL);
	pthread_join(quadrados_v[i], NULL);
  }
  if (flag_errado == 0)
    printf("Solucao correta!\n");

  return 0;
}


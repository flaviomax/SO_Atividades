/* Usa 3 threads para verificar se um jogo de sudoku foi completado corretamente: uma analisa por linhas,
outra por colunas e outra por quadrados 3 x 3 */

#include <pthread.h>
#include <stdio.h>

volatile int tabuleiro[9][9];
volatile int flag_errado = 0;

/** Thread que verifica se a solução está correta linha a linha */
void* f_thread_linhas(void *v) {
  int i, j;
  int boolean_ocorrencias[9] = {0}; 
  for (i = 0; i < 9; i++){
    for (j = 0; j < 9; j++){
      if (boolean_ocorrencias [tabuleiro [i][j] - 1] == 0)
        boolean_ocorrencias [tabuleiro [i][j] - 1] = 1;
      else{
        flag_errado = 1; /* Já há uma ocorrencia do numero na linha */
        printf("Mais de uma ocorrencia do numero %d na linha %d\n", tabuleiro [i][j], (i + 1));
      }
    }
    for (j = 0; j < 9; j++)
      boolean_ocorrencias[j] = 0;
  }
  return NULL;
}

/** Verifica a solução coluna a coluna */
void* f_thread_colunas(void *v) {
  int i, j;
  int boolean_ocorrencias[9] = {0}; 
  for (i = 0; i < 9; i++){
    for (j = 0; j < 9; j++){
      if (boolean_ocorrencias [tabuleiro [j][i] - 1] == 0)
        boolean_ocorrencias [tabuleiro [j][i] - 1] = 1;
      else{
        flag_errado = 1;
        printf("Mais de uma ocorrencia do numero %d na coluna %d\n", tabuleiro [j][i], (i + 1));
      }
    }
    for (j = 0; j < 9; j++)
      boolean_ocorrencias[j] = 0;
  }
  return NULL;
}

/** Verifica a solução a cada quadrado 3x3. Os quadrados são contados
da esquerda para a direita, de cima para baixo */
void* f_thread_quadrados(void *v) {
  int i, j, x, y;
  int boolean_ocorrencias[9] = {0}; 
  for (x = 0; x < 9; x += 3){
  	for (y = 0; y < 9; y += 3){
			for (i = 0; i < 3; i++){
				for (j = 0; j < 3; j++){
				  if (boolean_ocorrencias [tabuleiro [i + x][j + y] - 1] == 0)
				    boolean_ocorrencias [tabuleiro [i + x][j + y] - 1] = 1;
				  else{
				    flag_errado = 1;
				    printf("Mais de uma ocorrencia do numero %d no quadrado %d\n", tabuleiro [i + x][j + y], ( (x + 1) + (y/3) ) );
				  }
				}
			}
			for (j = 0; j < 9; j++)
				boolean_ocorrencias[j] = 0;
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

  pthread_t linhas, colunas, quadrados;
  
  if (pthread_create(&linhas, NULL, f_thread_linhas, NULL))
    fprintf(stderr, "Erro na criacao da thread. \n");
  if (pthread_create(&colunas, NULL, f_thread_colunas, NULL))
    fprintf(stderr, "Erro na criacao da thread. \n");
  if (pthread_create(&quadrados, NULL, f_thread_quadrados, NULL))
    fprintf(stderr, "Erro na criacao da thread. \n");

  pthread_join(linhas, NULL);
  pthread_join(colunas, NULL);
  pthread_join(quadrados, NULL);

  
  if (flag_errado == 0)
    printf("Solucao correta!\n");

  return 0;
}


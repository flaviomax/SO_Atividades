/* Usa 3 threads para verificar se um jogo de sudoku foi completado corretamente: uma analisa por linhas,
outra por colunas e outra por quadrados 3 x 3 */

#include <pthread.h>
#include <stdio.h>

volatile int tabuleiro[9][9];
volatile ocorrencias_linhas[9][9] = {9};
volatile ocorrencias_colunas[9][9] = {9};
volatile ocorrencias_quadrados[9][9] = {9};

/** Thread que verifica se a solução está correta linha a linha */
void* f_thread_linhas(void *v) {
  int i, j;
  int boolean_ocorrencias[9] = {0}; 
  for (i = 0; i < 9; i++){
    for (j = 0; j < 9; j++){
      if ( tabuleiro[i][j] != 0 ) {
        if (boolean_ocorrencias [tabuleiro [i][j] - 1] == 0)
          boolean_ocorrencias [tabuleiro [i][j] - 1] = 1;
      }
    }
    for (j = 0; j < 9; j++) 
        ocorrencias_linhas[i][j] = boolean_ocorrencias[j];
 
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
      if ( tabuleiro[j][i] != 0 ) {
        if (boolean_ocorrencias [tabuleiro [j][i] - 1] == 0)
          boolean_ocorrencias [tabuleiro [j][i] - 1] = 1;
      }
    }
    for (j = 0; j < 9; j++) 
        ocorrencias_colunas[i][j] = boolean_ocorrencias[j];

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
				  if ( tabuleiro[i + x][j + y] != 0 ) {
            if (boolean_ocorrencias [tabuleiro [i + x][j + y] - 1] == 0)
              boolean_ocorrencias [tabuleiro [i + x][j + y] - 1] = 1;
          }
				}
			}
      for (j = 0; j < 9; j++) 
        ocorrencias_quadrados[(x) + (y/3)][j] = boolean_ocorrencias[j];

			for (j = 0; j < 9; j++)
				boolean_ocorrencias[j] = 0;
		}
  }
  return NULL;
}

/*Verifica se a possibilidade de preenchimento já foi anotada*/
int n_ocorreu(int n, int poss[9]) {
  int i;
  for ( i = 0; i < 9; i++) {
    if (poss[i] == n)
      return 0;
  }
  return 1;
}

/*Imprime as possibilidades de cada posicao*/
void imprime_possibilidades(int poss[9]) {
  int i;  
  printf("(");
  
  for (i = 0; i < 9; i++){
    if (poss[i] == 0)
      printf ("%d", (i+1));
  }
  printf(") ");    
}

int main() {
  int possibilidades[9] = {0};
  int z, k = 0;

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


/*Imprimi possibilidades*/
  
  for (i = 0; i < 9; i++){
    for (j = 0; j < 9; j++){
      
      if (tabuleiro[i][j] != 0) {
        printf("%d ", tabuleiro[i][j]);
        continue;
      }
     
      for (k = 0; k < 9; k++){
        if (ocorrencias_linhas[i][k] != 0){           
            possibilidades[k] = 1;          
        }
      }  
      for (k = 0; k < 9; k++) {  
        if (ocorrencias_colunas[j][k] != 0){           
            possibilidades[k] = 1;
          }
         
      }
      for (k = 0; k < 9; k++) {
        if (ocorrencias_quadrados[(i/3*3 + j/3)][k] != 0){
          possibilidades[k] = 1;         
        }        
      }

      imprime_possibilidades(possibilidades);
      for (k = 0; k < 9; k++)
        possibilidades[k] = 0;
      z = 0;
    }
    printf("\n");
  }   

  return 0;
}


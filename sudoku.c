/*
 * CriaÃ§Ã£o de uma nova thread. SerÃ¡ que ela serÃ¡ sempre executada?
 */

#include <pthread.h>
#include <stdio.h>

volatile int tabuleiro[9][9];

void* f_thread_imprimir(void *v) {
  int i, j;
  for (i = 0; i < 9; i++){
    for (j = 0; j < 9; j++)
      printf ("%d ", tabuleiro[i][j]);

  printf("\n");
  }

  return NULL;
}

int main() {
  int i, j;
  for (i = 0; i < 9; i++)
    for (j = 0; j < 9; j++){
      char temp;
      scanf("%c ", &temp);
      if (temp != 'X')
	tabuleiro [i][j] = (temp - '0');
      else
        tabuleiro [i][j] = 0;
    }


  pthread_t thr;
  
  if (pthread_create(&thr, NULL, f_thread_imprimir, NULL))
    fprintf(stderr, "Erro na criacao da thread. \n");

  pthread_join(thr, NULL);

  return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#define ORDER 9

typedef enum{false, true}bool;
typedef struct{int x; int y;}point;

int matrix[ORDER][ORDER];
int emptys_size;
point* emptys;

bool volatile foundSolution;
bool volatile finishProgram;
int volatile id_first;


int convertChar(char c){
	if (c == 'X')
		return 0;
	else
		return c - '0';
}

void readMatrixFromFile(){
	FILE *fin  = fopen ("sudoku.in", "r");
	int i,j;
	char c;
	for (i=0; i<ORDER; ++i){
		for(j=0; j<ORDER; ++j){
			fscanf(fin," %c",&c);
			matrix[i][j] = convertChar(c);
		}
	}
}

void printMatrix(int m[][ORDER]){
	int i,j;
	for (i=0; i<ORDER; ++i){
		for(j=0; j<ORDER; ++j){
			printf("%d ",m[i][j]);
		}
		printf("\n");
	}
}

void setArrayWithEmptyPositions(){
  emptys = malloc(sizeof(point)*(ORDER*ORDER));
  emptys_size = 0;
  int i,j;
  for(i=0; i<ORDER; ++i){
     for(j=0; j<ORDER; ++j){
       if(matrix[i][j] == 0){
	 point p;
	 p.x = i; p.y = j;
	 emptys[emptys_size++] = p;
       }
     }
  }
}

point getPositionForStartPosition(int pos, int startPos){
  point actuallyPos = emptys[(pos + startPos)%emptys_size];
  return actuallyPos;
}

bool checkRow(int m[][ORDER], int row, int value){
	int j;
	for(j=0; j<ORDER; ++j) 
		if (m[row][j] == value)
			return false;
	return true;
}

bool checkColumn(int m[][ORDER], int column, int value){
	int i;
	for(i=0; i<ORDER; ++i) 
		if (m[i][column] == value)
			return false;
	return true;
}

bool checkQuadrant(int m[][ORDER], point pos, int value){
	int row = pos.x;
	int column = pos.y;
	int order_sqrt = sqrt(ORDER);
	int initialRowPosition = row/order_sqrt * order_sqrt;
	int initialColumnPosition = column/order_sqrt * order_sqrt;
	int i,j;

	 for(i = initialRowPosition; i < initialRowPosition + order_sqrt; ++i)
	    for(j = initialColumnPosition; j < initialColumnPosition + order_sqrt; ++j)
			if(m[i][j] == value)
				return false;
	 return true;
}


bool isInsertionValid(int m[][ORDER], point p, int value){
	return ( checkRow(m,p.x,value) && checkColumn(m,p.y,value) && checkQuadrant(m,p,value) );
}

void solveRec(int m[][ORDER], int startPos, int num_pos, int value){
	point p = getPositionForStartPosition(num_pos, startPos);
	
	if(!foundSolution){
		for(;value<=ORDER && !isInsertionValid(m,p,value); ++value);
		if(value <= ORDER){
			m[p.x][p.y] = value;
			if(num_pos + 1 >= emptys_size){
				id_first = startPos;
				foundSolution = true;	// JA FINALIZEI O PREECHIMENTO
			}
			else{
				solveRec(m,startPos,num_pos + 1,1);
				//VOLTA
				if(!foundSolution){
					value = m[p.x][p.y] + 1;
					m[p.x][p.y] = 0;
					solveRec(m,startPos,num_pos,value);
				}
			}
		}
	}	
  

}

void* solveSudoku(void* start){
	int matrix_copy[ORDER][ORDER];
	int i,j;
	int startPosition = *((int*)start);
	for(i=0; i<ORDER; ++i){
		for(j=0; j<ORDER; ++j){
			matrix_copy[i][j] = matrix[i][j];
		}
	}
	
	double clockA = clock();
	
	solveRec(matrix_copy, startPosition,0,1);
	if(id_first == startPosition){
		printf("\nA THREAD PARTINDO DA POSICAO %d FOI A PRIMEIRA A TERMINAR! (%lf s) \n\n",startPosition,(clock()-clockA)/CLOCKS_PER_SEC);
		printMatrix(matrix_copy);
		finishProgram = true;
	}
	
	return NULL;
}

int main(){
	readMatrixFromFile();
	printMatrix(matrix);
	
	setArrayWithEmptyPositions();
	
	finishProgram = false;
	pthread_t thr[emptys_size];
	int starts[emptys_size];
	id_first = -1;
	
	int startPos;
	for(startPos=0; startPos<emptys_size; ++startPos){
		starts[startPos] = startPos;
		pthread_create(&thr[startPos],NULL,solveSudoku,(void*) &starts[startPos]);
	}
	
	while(!finishProgram);
	return 0;
}
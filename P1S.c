#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
  srand(time(NULL));
  int mX,mY,vX,vY;
  bool control = true;

  // Initial matrix and vector size request
  while(control){
    printf("Ingrese la dimensión de la matriz. Formato:x,y\n");
    scanf("%d,%d",&mX,&mY );
    printf("Ingrese la dimensión del vector COLUMNA. Formato:x,y\n");
    scanf("%d,%d",&vX,&vY );
    if (vY!=1){
      printf("ERROR datos ingresados no corresponden a un vector columna\n");
    }else if ( mY != vX) {
        printf("Las matrices ingresadas no se pueden multiplicar, intente de nuevo\n");
      } else {
        control = false; // if inputs are usable , continue with multiplication
      }
  }
  int matrix[mX][mY]; // Matrix with input sizes
  int vector[vX][vY]; // Vector with input sizes
  for( int i = 0 ; i < mX ; i++){
    for (int j = 0; j < mY; j++) {
      matrix[i][j]= rand()%10; // Fill matrix with random numbers from 0-9
      if(j<vY && i<vX){
        vector[i][j]= rand()%10; // while filling vector array as well
      }
    }
  }

  //Print generated vectors

  printf("Matriz = ( %d , %d )\n", mX , mY);
  for(int i = 0; i < mX; i++) {
    printf("\t|");
    for(int j = 0; j < mY; j++) {
        printf("%d ", matrix[i][j]);
    }
    printf("|\n");
  }
  printf("Vector = ( %d , %d )\n", vX,vY);
  for(int i = 0; i < vX; i++) {
    printf("\t|");
    for(int j = 0; j < vY; j++) {
        printf("%d ", vector[i][j]);
    }
    printf("|\n");
  }

  int resultArr[mX][1];
  for(int j = 0; j < mX; j++) {
      resultArr[j][0] = 0;
  }
  int resultado;
  for (int i = 0; i < mX; i++) {
    for (int j = 0; j < mY; j++) {
      resultArr[i][0] += (matrix[i][j])*(vector[j][0]);//resultArr[posI][posJ]+resultado;
      printf("Multiplicando %d x %d \t",matrix[i][j],vector[j][0]);
      printf("resArray pos %d , %d = %d \n",i,0, resultArr[i][0] );
    }
  }

  printf("Matriz resultante = ( %d , %d )\n", mX,0);
  printf("\t|");
    for(int j = 0; j < mX; j++) {
        printf("%d ", resultArr[j][0]);
    }
    printf("|\n");

  return 0;
}

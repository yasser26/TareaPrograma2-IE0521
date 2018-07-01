/*Realiza una multiplicación de una matriz y un vecor columna en forma secuencial , recibe las dimensiones 
de los arreglos y tiene por salida el resultado de la multiplicación  */
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
        control = false; // Si los datos ingresados son correctos, continúa la ejecución
      }
  }
  int matrix[mX][mY]; 
  int vector[vX][vY]; 
  for( int i = 0 ; i < mX ; i++){
    for (int j = 0; j < mY; j++) {
      matrix[i][j]= rand()%10; // Llena la matriz con numeros de 0-9
      if(j<vY && i<vX){
        vector[i][j]= rand()%10; // y el vector también 
      }
    }
  }

  //PImprime los vectores generados

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
  // Multiplica los vectores
  int resultado;
  for (int i = 0; i < mX; i++) {
    for (int j = 0; j < mY; j++) {
      resultArr[i][0] += (matrix[i][j])*(vector[j][0]);//resultArr[posI][posJ]+resultado;
      printf("Multiplicando %d x %d \t",matrix[i][j],vector[j][0]);
      printf("resArray pos %d , %d = %d \n",i,0, resultArr[i][0] );
    }
  }
  // Imprime el resultado 
  printf("Matriz resultante = ( %d , %d )\n", mX,0);
  printf("\t|");
    for(int j = 0; j < mX; j++) {
        printf("%d ", resultArr[j][1]);
    }
    printf("|\n");

  return 0;
}

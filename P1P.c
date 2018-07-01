/* El siguiente programa realiza la multiplicación de un vector columna con una matriz y tiene como entradas las dimensiones
de dichos arreglos , y por salida el vector resultante  
*/
#include <mpi.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const **argv) {
  int rank, root_process, ierr, size,  mX,mY,vX,vY , row ,i ,j ;
  bool control = true;
  MPI_Status status;
  ierr = MPI_Init(&argc, &argv); // Iniciar la aplicacion paralela
  ierr = MPI_Comm_rank(MPI_COMM_WORLD,&rank); // Identifica numero de procesos de la aplicacion
  ierr = MPI_Comm_size(MPI_COMM_WORLD,&size); //Permote qie cada proceso identifique su direccion dentro dela coleccion

  srand(time(NULL));

  /*Master pide los datos necesarios y crea las matrices con números aleatorios */
  if(rank == 0){
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
          control = false; // Si los datos incresados son correctos, continúa con la ejecución
        }
    }

  int matrix[mX][mY]; 
  int vector[vX][vY]; 
  for( i = 0 ; i < mX ; i++){
    for ( j = 0; j < mY; j++) {
      matrix[i][j]= rand()%10; // Se utilizan números aleatorios del 0-9
      if(j<vY && i<vX){
        vector[i][j]= rand()%10; // y al mismo tiempo se llena el vector
      }
    }
  }
  // Imprimir vectores generados
  printf("Matriz = ( %d , %d )\n", mX , mY);
  for( i = 0; i < mX; i++) {
    printf("\t|");
    for( j = 0; j < mY; j++) {
        printf("%d ", matrix[i][j]);
    }
    printf("|\n");
  }
  printf("Vector = ( %d , %d )\n", vX,vY);
  for(i = 0; i < vX; i++) {
    printf("\t|");
    for( j = 0; j < vY; j++) {
        printf("%d ", vector[i][j]);
    }
    printf("|\n");
  }

  int resultArr[mX];
  for( j = 0; j < mX; j++) {
      resultArr[j] = 0;
  }
  int currentProc = 1 , sendSize = size,initial=0 ;
  row = mX/size;
  for (i = 1; i < size; i++)  {
      MPI_Send(&mY,1,MPI_INT, i , initial , MPI_COMM_WORLD);// Se envía el valor de las columnas
      MPI_Send(&row,1,MPI_INT, i , initial , MPI_COMM_WORLD); //y el número de filas a revisar por proceso
      MPI_Send(&matrix[initial],mY*row,MPI_INT, i , initial , MPI_COMM_WORLD);// envío las  filas a multiplicar 
      MPI_Send(&vector,mY,MPI_INT, i , initial , MPI_COMM_WORLD); // y el vector que multiplica
      //printf("Initial:%d, CP %d , row %d\n",initial,currentProc , row);
      initial += row ; // cada procesador recibe la cantidad de filas en row 
      sendSize = sendSize - row;
      currentProc += 1;
      row = sendSize;
    }
    int row1 ;
    // En esta función se reciben los datos y se guarda en la matriz de resultado 
    for (i = 1; i < size; i++) {
        ierr = MPI_Recv(&row1,1,MPI_INT,MPI_ANY_SOURCE, MPI_ANY_TAG , MPI_COMM_WORLD, &status);//
        int resultado[row1];
        ierr = MPI_Recv(&resultado,row1,MPI_INT,MPI_ANY_SOURCE, MPI_ANY_TAG , MPI_COMM_WORLD, &status);//
        int num = status.MPI_TAG;
        for (int c = 0; c < row1; c++) {
          //printf("Receiving %d from %d , kept in %d \n", resultado[c],status.MPI_SOURCE,num);
          resultArr[num]=resultado[c];
          num = num +1;
        }

      }

  printf("Matriz resultante = ( %d , %d ) \n", mX,0);
    printf("\t|");
    for( j = 0; j < mX; j++) {
        printf("%d ", resultArr[j]);
    }
    printf("|\n");


}else {
  //   MPI_Recv(&count, 1, MPI_INT, rank_opposite, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  MPI_Recv(&mY,1,MPI_INT, 0 , MPI_ANY_TAG , MPI_COMM_WORLD, &status);//
  MPI_Recv(&row,1,MPI_INT, 0 , MPI_ANY_TAG , MPI_COMM_WORLD, &status);//
  int resultado[row];
  //printf("Recieved %d and %d in %d \n", mY ,row, rank );
  int recMatrix[row][mY];
  int vector[mY];
  MPI_Recv(&recMatrix,mY*row,MPI_INT,0, MPI_ANY_TAG , MPI_COMM_WORLD , &status);//
  MPI_Recv(&vector,mY,MPI_INT,0, MPI_ANY_TAG , MPI_COMM_WORLD, &status);

  // con los datos ingresados se realiza la multiplicación matricial
  for ( j = 0; j < row; j++) {
    resultado[j] = 0;
    for ( i = 0; i < mY; i++) {
        resultado[j] += (vector[i])*(recMatrix[j][i]);
      //  printf("%dx%d=%d Proc: %d\n", vector[i],recMatrix[j][i],resultado[j], rank);
    }

  }
  MPI_Send(&row,1,MPI_INT, 0 , status.MPI_TAG , MPI_COMM_WORLD);
  MPI_Send(&resultado,row,MPI_INT, 0 , status.MPI_TAG , MPI_COMM_WORLD);
}

  MPI_Finalize();
  return 0;
}

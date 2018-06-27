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

  /*Master creates matrix and */
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
          control = false; // if inputs are usable , continue with multiplication
        }
    }

  int matrix[mX][mY]; // Matrix with input sizes
  int vector[vX][vY]; // Vector with input sizes
  for( i = 0 ; i < mX ; i++){
    for ( j = 0; j < mY; j++) {
      matrix[i][j]= rand()%10; // Fill matrix with random numbers from 0-9
      if(j<vY && i<vX){
        vector[i][j]= rand()%10; // while filling vector array as well
      }
    }
  }
  // Print Generated Vectors
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
      MPI_Send(&mY,1,MPI_INT, i , initial , MPI_COMM_WORLD);// send Y max
      MPI_Send(&row,1,MPI_INT, i , initial , MPI_COMM_WORLD); //send amount of rows
      MPI_Send(&matrix[initial],mY*row,MPI_INT, i , initial , MPI_COMM_WORLD);// send each line with mY values
      MPI_Send(&vector,mY,MPI_INT, i , initial , MPI_COMM_WORLD); // vector
      //printf("Initial:%d, CP %d , row %d\n",initial,currentProc , row);
      initial += row ;
      sendSize = sendSize - row;
      currentProc += 1;
      row = sendSize;
    }
    int row1 ;
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

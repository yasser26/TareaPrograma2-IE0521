
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

// Definicion de variable usada durante la ejecución del programa
#define limitNum 7368788

// Función que determina si un número es primo o no, para ello
// se va obteniendo el modulo desde el num 3 hasta la raiz del
// numero analizado. Se saltan los numeros pares
int determinatePrimeNum(int n) {
int i,squareroot;
if (n>10) {
   squareroot = (int) sqrt(n);
   for (i=3; i<=squareroot; i=i+2)
      if ((n%i)==0)
         return 0;
   return 1;
   }
else
   return 0;
}

// Funcion principal main()
int main (int argc, char *argv[])
{
int   size, rank, n, numsPrime, numsPrimeTotal, foundone, maxprime;
double startTime,endTime;

MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD,&rank);
MPI_Comm_size(MPI_COMM_WORLD,&size);


startTime = MPI_Wtime();
numsPrime=0;
foundone = 0;

// El nodo master utiliza la funcion MPI_Reduce() para ir sumando la cantidad de números primos
// que van encuentrando los nodos trabajadores. Al igual que el numero primo mayor
if (rank == 0) {
   printf("Usando %d procesos para determinar los primeros %d numeros\n",size,limitNum);
   numsPrime = 4;
   for (n=(rank*2)+1; n<=limitNum; n=n+size*2) {
      if (determinatePrimeNum(n)) {
         numsPrime++;
         foundone = n;

         printf("%d\n",foundone);

       }
   }
  // se van sumando la cantidad de numeros primos encontrados y el maximo de ellos
   MPI_Reduce(&numsPrime,&numsPrimeTotal,1,MPI_INT,MPI_SUM, 0,MPI_COMM_WORLD);
   MPI_Reduce(&foundone,&maxprime,1,MPI_INT,MPI_MAX, 0,MPI_COMM_WORLD);
   endTime=MPI_Wtime();

   // Se imprime el resultado
   printf("El numero primo mas alto es: %d . Numero total de primos:  %d\n",maxprime,numsPrimeTotal);
   printf("Tiempo de ejecución: %.2lf s\n",endTime-startTime);
   }


// Los nodos trabajadores van determinando los numero primos de forma paralela e usan la funcion
// MPI_Reduce para enviarla al nodo master
else {
   for (n=(rank*2)+1; n<=limitNum; n=n+size*2) {
      if (determinatePrimeNum(n)) {
         numsPrime++;
         foundone = n;
         }
      }
   MPI_Reduce(&numsPrime,&numsPrimeTotal,1,MPI_INT,MPI_SUM, 0,MPI_COMM_WORLD);
   MPI_Reduce(&foundone,&maxprime,1,MPI_INT,MPI_MAX, 0,MPI_COMM_WORLD);
   }

MPI_Finalize();
}

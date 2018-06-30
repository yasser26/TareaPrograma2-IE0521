
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define limitNum 7368788    /* Increase this to find more primes */

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

/******************** task with rank 0 does this part ********************/
if (rank == 0) {


   printf("Usando %d procesos para determinar los primeros %d numeros\n",size,limitNum);
   numsPrime = 4;                  /* Assume first four primes are counted here */
   for (n=(rank*2)+1; n<=limitNum; n=n+size*2) {
      if (determinatePrimeNum(n)) {
         numsPrime++;
         foundone = n;
         /***** Optional: print each prime as it is found
         printf("%d\n",foundone);
         *****/
         printf("%d\n",foundone);

         }
      }
   MPI_Reduce(&numsPrime,&numsPrimeTotal,1,MPI_INT,MPI_SUM, 0,MPI_COMM_WORLD);
   MPI_Reduce(&foundone,&maxprime,1,MPI_INT,MPI_MAX, 0,MPI_COMM_WORLD);
   endTime=MPI_Wtime();
   printf("El numero primo mas alto es: %d . Numero total de primos:  %d\n",maxprime,numsPrimeTotal);
   printf("Tiempo de ejecución: %.2lf s\n",endTime-startTime);
   }


/******************** all other tasks do this part ***********************/
else {
   for (n=(rank*2)+1; n<=limitNum; n=n+size*2) {
      if (determinatePrimeNum(n)) {
         numsPrime++;
         foundone = n;
         /***** Optional: print each prime as it is found
         printf("%d\n",foundone);
         *****/

         }
      }
   MPI_Reduce(&numsPrime,&numsPrimeTotal,1,MPI_INT,MPI_SUM, 0,MPI_COMM_WORLD);
   MPI_Reduce(&foundone,&maxprime,1,MPI_INT,MPI_MAX, 0,MPI_COMM_WORLD);
   }

MPI_Finalize();
}

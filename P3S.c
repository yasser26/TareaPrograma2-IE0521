/*Input: Cantidad de números primos a encontrar
  Output: Tiempo de ejecución para encontrar los primos
  Si quiere ver la impresión de los números, descomentar la línea 36*/

#include<stdio.h>
#include <time.h>
#include <math.h>

int main()
{
    int n,i = 3, count, c;

    printf("\nIngrese el número de primos deseados:  ");
    scanf("%d", &n);

    // iteración de números primos
    // siendo i el número que se revisa comenzando en el 3
    clock_t t;
    t = clock();


    for(count = 2; count <= n; i++)
    {
      if(i%2!=0 ){ // Me salto los número múltiplo de 2 porque todos son compuestos
        // iteracion para revisar si es primo o no
        for(c = 2; c <= sqrt(i); c++)
        {
          if(c%2 != 0){
            if(i%c == 0) // aplico el módulo con los números anteriores a él distinto de múltiplos de 2 porque esos no los evalúo
                break;
              }
        }

        if(c == (int) sqrt(i)+1)  // si no es divisible entre ninguno de los numeros anteriores o igual a la raiz cuadrada del número entonces es primo
        {
          // printf("%d ", i); //Para imprimir números descomentar esta línea
            count++;    // incrementa el contador de números primos
        }

      }
    }
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // Finalizo la cuenta de tiempo de ejecución
    printf("La función tomó %f segundos en ejecutarse \n", time_taken);
    return 0;
}

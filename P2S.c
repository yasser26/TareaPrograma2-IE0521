

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Definicion de variables usadas durante la ejecución del programa
int m_row=8, m_column=8, visited[8], bestPath[8];
char Locations [8][14] = {"San Jose", "Limon   ", "San Francisco", "Alajuela", "Liberia   ", "Paraiso   ", "Puntarenas", "San Isidro"};
int distLocations [8][8] = {{0, 115, 8, 17, 167, 26, 83, 75},
                              {115, 0, 120, 129, 272, 92, 197, 100},
                              {8, 120, 0, 9, 160, 34, 78, 83},
                              {17, 129, 9, 0, 151, 43, 69, 91},
                              {167, 272, 160, 151, 0, 193, 98, 236},
                              {26, 92, 34, 43, 193, 0, 108, 55},
                              {83, 197, 78, 69, 98, 108, 0, 141},
                              {75, 100, 83, 91, 236, 55, 141, 0}};


int bestDist=9999999, size=8;

void checkShortestDistance(int location, int visited_in[], int path_in[], int path_i_in, int dist_in);

// La funcion checkShortestDistance() determina de forma recursiva, el camino mas
// corte partiendo de una cuidad, a las otras cuidades.
void checkShortestDistance(int location, int visited_in[], int path_in[], int path_i_in, int dist_in) {
  // condición se cumple si se está ante una distancia menor a la mejor distancia guardada
  if (dist_in < bestDist) {
    int *visited=calloc(sizeof(int),size+1);
    int *path=calloc(sizeof(int),size+1);
    int path_i=path_i_in;
    int dist=dist_in;
    int i;

    // Se actualizan la lista de cuidades ya visitadas (visited[]) y
    // la ruta más corta hasta el momento (path[])
    for (i=0; i<size; i++) {
      visited[i]=visited_in[i];
      path[i]=path_in[i];
    }

    // Se marca la nueva mejor ruta como visitada y se agrega al path
    visited[location]=1;
    path[path_i]=location;
    path_i++;

    // Variable de parada de la recursividad
    int varia=0;
    // Se aplica la recursividad para cada cuidad
    for(i=0; i<size; i++) {
      if(visited[i]==0) {
        varia++;
        checkShortestDistance(i, visited, path, path_i, dist+distLocations[location][i]);
      }
    }

    // Condición de parada que índica la llegada a la cuidad de inicial de partida
    // se actualizan las varibles bestPath[], bestDist
    if (varia == 0) {
      dist+=distLocations[location][0];
      path[path_i]=0;
      path_i++;

      if(dist < bestDist) {
        bestDist=dist;
        for(i=0; i<size; i++)
        bestPath[i]=path[i];
      }
    }

    free(visited);
    free(path);
  }
}



// Función principal main()
int main(int argc, char *argv[]) {

// Se imprime la tabla de distancias
  printf("\t\t");
  for (int i = 0; i < 8; i++) {
    printf("%s\t", Locations[i] );
  }
  printf("\n");

  for (int r = 0; r < 8; r++) {
    printf("%s\t", Locations[r] );
    for (int c = 0; c < 8; c++) {
      printf("%d\t\t", distLocations[r][c]);
    }
    printf("\n");
  }

// Se determina la ruta más corta, entre las 8 cuidades
  for (int i = 0; i < 8; i++) {
    int *visited = calloc(sizeof(int),size+1);
    int *path = calloc(sizeof(int),size+1);
    int dist = distLocations[0][i], path_i=1;
    path[0] = 0;
    visited[0] = 1;

    checkShortestDistance(i, visited, path, path_i, dist);

    free(visited);
    free(path);
}

// Se imprime los resultados
  for(int i = 1; i < size+1; i++)
    printf("%i –>",bestPath[i]);

  printf("\nLa distancia mas corta es de: %i\n", bestDist);

  return 0;
}

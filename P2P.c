#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

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
int main(int argc, char* argv[]) {

  // Definicion de variables para MPI
  int rank, p, source, dest;
  int tag=0;
  MPI_Status status;

  MPI_Init(0, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // El nodo master recive las rutas de los demás nodos(workers), determina cual es el mejor camino
  // e imprime el resultado en terminal
  if (rank == 0) {
    int i, j;
    int winner;
    int node_array[p-1];
    int node_array_i=0;

    // Se imprime la table
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

    // Envía la tabla de distancias a los demás nodos
    for(i=1; i<p; i++)
      MPI_Send(&distLocations[0][0], size*size, MPI_LONG, i, 0, MPI_COMM_WORLD);

    printf("\n");

    // Se guardan los procesos en un arreglo
    for(i = 0; i < p-1; i++)
      node_array[i] = i + 1;

    // Se determina el path más corto entre todos los nodos
    for(i = 1; i < size; i++) {
      int temp_bestDist, node;
      int *temp_bestPath=calloc(sizeof(int),size+1);

      node = node_array[node_array_i];

      if (node_array_i < p-2)
        node_array_i++;
      else
        node_array_i = 0;

      // Recive el path de los nodos trabajadores
      MPI_Recv(&temp_bestDist, 1, MPI_INT, node, 0, MPI_COMM_WORLD, &status);
      MPI_Recv(&temp_bestPath[0], size+1, MPI_INT, node, 0, MPI_COMM_WORLD, &status);

      // Determina el path más corto
      if(temp_bestDist < bestDist) {
        winner = node;
        bestDist = temp_bestDist;

        for(j = 0; j < size+1; j++)
          bestPath[j] = temp_bestPath[j];
      }

      // Se envía al nodo que tiene la ruta más corta, la distancia mas corta
      MPI_Send(&bestDist, 1, MPI_INT, node, 0, MPI_COMM_WORLD);
    }

    // Imprime la ruta más corta encontrada y el nodo que la encontró
    printf("Ruta mas corta encontrada por el proceso: %i:\n", winner);
    printf("%i",bestPath[0]);

    for(i = 1; i < size+1; i++)
      printf(" –> %i",bestPath[i]);

    printf("\nLa distancia mas corta es: %i\n", bestDist);

  }

  // Los nodos trabajadores reciben la matriz de distancias e utilizan la función
  // checkShortestDistance() para determinadar la ruta mas corta de manera local (dentro del proceso)
  else {
    MPI_Recv(&(distLocations[0][0]), m_row*m_column, MPI_LONG, 0, 0, MPI_COMM_WORLD, &status);
    int i;

    // Se distribuye la revision de las cuidades entre todos los procesos
    for(i = rank; i < size; i+=(p-1)) {
      int *visited = calloc(sizeof(int),size+1);
      int *path = calloc(sizeof(int),size+1);
      int dist = distLocations[0][i], path_i=1;
      path[0] = 0;
      visited[0] = 1;

      // se determina la ruta mas corta
      checkShortestDistance(i, visited, path, path_i, dist);

      // Se envía la mejor ruta con la menor distancia al nodo master
      MPI_Send(&bestDist, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
      MPI_Send(&bestPath[0], size+1, MPI_INT, 0, 0, MPI_COMM_WORLD);

      // Se esta recibiendo del nodo master cual es la mejor ruta encontrada hasta el momento
      MPI_Recv(&bestDist, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

      free(visited);
      free(path);
    }
  }

// Se finaliza Open MPI
MPI_Finalize();
return 0;
}

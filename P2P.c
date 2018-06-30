#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


int m_row=8, m_column=8, visited[8], best_path[8];
char Locations [8][14] = {"San Jose", "Limon   ", "San Francisco", "Alajuela", "Liberia   ", "Paraiso   ", "Puntarenas", "San Isidro"};
int distLocations [8][8] = {{0, 115, 8, 17, 167, 26, 83, 75},
                            {115, 0, 120, 129, 272, 92, 197, 100},
                            {8, 120, 0, 9, 160, 34, 78, 83},
                            {17, 129, 9, 0, 151, 43, 69, 91},
                            {167, 272, 160, 151, 0, 193, 98, 236},
                            {26, 92, 34, 43, 193, 0, 108, 55},
                            {83, 197, 78, 69, 98, 108, 0, 141},
                            {75, 100, 83, 91, 236, 55, 141, 0}};

int best_cost=9999999, size=8;

void checkShortestDistance(int city, int visited_in[], int path_in[], int path_i_in, int cost_in);

void checkShortestDistance(int city, int visited_in[], int path_in[], int path_i_in, int cost_in) {
  if (cost_in < best_cost) {
    int *visited=calloc(sizeof(int),size+1);
    int *path=calloc(sizeof(int),size+1);
    int path_i=path_i_in;
    int cost=cost_in;
    int i;

    for (i=0; i<size; i++) {
      visited[i]=visited_in[i];
      path[i]=path_in[i];
    }

    visited[city]=1;
    path[path_i]=city;
    path_i++;

    int leaf=0;
    for(i=0; i<size; i++) {
      if(visited[i]==0) {
        leaf++;
        checkShortestDistance(i, visited, path, path_i, cost+distLocations[city][i]);
      }
    }

    if (leaf == 0) {
      cost+=distLocations[city][0];
      path[path_i]=0;
      path_i++;

      if(cost < best_cost) {
        best_cost=cost;
        for(i=0; i<size; i++)
        best_path[i]=path[i];
      }
    }

    free(visited);
    free(path);
  }
}


// main() function
int main(int argc, char* argv[]) {

  int rank, p, source, dest;
  int tag=0;
  MPI_Status status;

  MPI_Init(0, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // The master node receives the paths of the other nodes (workers), determines which is the best path
  // and prints the result in terminal
  if (rank == 0) {
    int i, j;
    int winner;
    int node_array[p-1];
    int node_array_i=0;

    // Print the table
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

    // Sends the table of distances to the workers
    for(i=1; i<p; i++)
      MPI_Send(&distLocations[0][0], size*size, MPI_LONG, i, 0, MPI_COMM_WORLD);

    printf("\n");


    for(i = 0; i < p-1; i++)
      node_array[i] = i + 1;

    for(i = 1; i < size; i++) {
      int temp_best_cost, node;
      int *temp_best_path=calloc(sizeof(int),size+1);

      node = node_array[node_array_i];

      if (node_array_i < p-2)
        node_array_i++;
      else
        node_array_i = 0;

      // Receives the best path and distance from the other nodes
      MPI_Recv(&temp_best_cost, 1, MPI_INT, node, 0, MPI_COMM_WORLD, &status);
      MPI_Recv(&temp_best_path[0], size+1, MPI_INT, node, 0, MPI_COMM_WORLD, &status);

      // Determinates if the recived path and distance is the shortest
      if(temp_best_cost < best_cost) {
        winner = node;
        best_cost = temp_best_cost;

        for(j = 0; j < size+1; j++)
          best_path[j] = temp_best_path[j];
      }

      MPI_Send(&best_cost, 1, MPI_INT, node, 0, MPI_COMM_WORLD);
    }

    // Prints the best path and distance. In addition, it shows the worker who has found it
    printf("Best path found by processor %i:\n", winner);
    printf("%i",best_path[0]);

    for(i = 1; i < size+1; i++)
      printf(" –> %i",best_path[i]);

    printf("\nShortest distance found: %i\n", best_cost);

  }

  // The workers nodes receives the table of distances and use the dfs() function
  else {
    MPI_Recv(&(distLocations[0][0]), m_row*m_column, MPI_LONG, 0, 0, MPI_COMM_WORLD, &status);
    int i;

    for(i = rank; i < size; i+=(p-1)) {
      int *visited = calloc(sizeof(int),size+1);
      int *path = calloc(sizeof(int),size+1);
      int cost = distLocations[0][i], path_i=1;
      path[0] = 0;
      visited[0] = 1;

      checkShortestDistance(i, visited, path, path_i, cost);

      MPI_Send(&best_cost, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
      MPI_Send(&best_path[0], size+1, MPI_INT, 0, 0, MPI_COMM_WORLD);

      MPI_Recv(&best_cost, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

      free(visited);
      free(path);
    }
  }

MPI_Finalize();
return 0;
}

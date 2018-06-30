/*0 1 2 10
1 0 3 5
2 3 0 6
10 5 6 0 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int m_row=8, m_column=8, visited[8], best_path[8];
///////////////////////// Table of locations and distances /////////////////////////
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

/*int least(int c);
void mincost(int city);


int least(int c)
{
    int i,nc=999;
    int min=999,kmin;

    for(i=0;i < n;i++)
    {
        if((distLocations[c][i]!=0)&&(completed[i]==0))
            if(distLocations[c][i]+distLocations[i][c] < min)
            {
                min=distLocations[i][0]+distLocations[c][i];
                kmin=distLocations[c][i];
                nc=i;
            }
    }

    if(min!=999)
        cost+=kmin;

    return nc;
}

void mincost(int city)
{
    int i,ncity;

    completed[city]=1;

    printf("%d--->",city+1);
    ncity=least(city);

    if(ncity==999)
    {
        ncity=0;
        printf("%d",ncity+1);
        cost+=distLocations[city][ncity];

        return;
    }

    mincost(ncity);
} */




int main(int argc, char *argv[]) {

///////////////////////// Print the table /////////////////////////
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

///////////////////////// Determinates de shortest distance /////////////////////////
/*  printf("\n\nPath:\t");
  printf("\n\nThe Path is:\n");
  mincost(0); //passing 0 because starting vertex
  printf("\n\nMinimum cost is %d\n ",cost); */
  for (int i = 0; i < 8; i++) {


  int *visited = calloc(sizeof(int),size+1);
  int *path = calloc(sizeof(int),size+1);
  int cost = distLocations[0][i], path_i=1;
  path[0] = 0;
  visited[0] = 1;

  checkShortestDistance(i, visited, path, path_i, cost);



  free(visited);
  free(path);

}


for(int i = 1; i < size+1; i++)
  printf("%i –>",best_path[i]);

printf("\nShortest distance found: %i\n", best_cost);
  return 0;
}




/* #include<stdio.h>

int ary[10][10],completed[10],n,cost=0;
int least(int c);
void takeInput();
void mincost(int city);

void takeInput()
{
    int i,j;

    printf("Enter the number of villages: ");
    scanf("%d",&n);

    printf("\nEnter the Cost Matrix\n");

    for(i=0;i < n;i++)
    {
        printf("\nEnter Elements of Row: %d\n",i+1);

        for( j=0;j < n;j++)
            scanf("%d",&ary[i][j]);

        completed[i]=0;
    }

    printf("\n\nThe cost list is:");

    for( i=0;i < n;i++)
    {
        printf("\n");

        for(j=0;j < n;j++)
            printf("\t%d",ary[i][j]);
    }
}

void mincost(int city)
{
    int i,ncity;

    completed[city]=1;

    printf("%d--->",city+1);
    ncity=least(city);

    if(ncity==999)
    {
        ncity=0;
      //  printf("%d",ncity+1);
        cost+=ary[city][ncity];

        return;
    }

    mincost(ncity);
}

int least(int c)
{
    int i,nc=999;
    int min=999,kmin;

    for(i=0;i < n;i++)
    {
        if((ary[c][i]!=0)&&(completed[i]==0))
            if(ary[c][i]+ary[i][c] < min)
            {
                min=ary[i][0]+ary[c][i];
                kmin=ary[c][i];
                nc=i;
            }
    }

    if(min!=999)
        cost+=kmin;

    return nc;
}

int main()
{
    takeInput();

    printf("\n\nThe Path is:\n");
    mincost(3); //passing 0 because starting vertex

    printf("\n\nMinimum cost is %d\n ",cost);

    return 0;
} */

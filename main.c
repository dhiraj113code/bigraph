//Graph biconnectivity 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "biconn.h"
#define FILE_NAME_SIZE 100
#define DEBUG FALSE


static int nVert = 0;
static int **adjlist; //Adjacency list representation of input vertices
static int *nEdges;
static Pvertex *vertices;

int main(int argc, char **argv)
{
FILE *input;
if(argc !=2) {printf("Wrong number of input arguments specified\n"); exit(-1);}
char *fname;
fname = (char*)malloc(sizeof(char)*FILE_NAME_SIZE);
fname = argv[1];
read_file(fname);
if(DEBUG) printAdjList();
initializeNallmem();
biconn(vertices[0]->node);
}



void biconn(int node)
{
  int i, index, test_node, test_node_index;
  printf("biconn is called with the node = %d\n", node);
  index = node - 1;
  //Loop over the vertices in the adjacency list for this node
  for(i = 1; i < nEdges[index] + 1; i++)
  {
     vertices[index]->color = GRAY;
     test_node = adjlist[index][i];
     test_node_index = test_node - 1;
     if(vertices[test_node_index]->color == WHITE)
        biconn(test_node);
  }
}



int read_file(char fname[])
{
   FILE * pFile;
   char mystring [100];
   long int l;
   char *pEnd;
   int i = 0, j = 0, k = 0, spaces = 0, lines = 0;

   //Determine number of lines in the file
   pFile = fopen(fname, "r");
   if(pFile == NULL)
       perror("Error opening file");
   else
   {
      while( fgets (mystring , 100 , pFile) != NULL )
      {
         lines++;
      }
   }
   fclose(pFile);

   nVert = lines;
   //Allocating memory for integers array pointers
   adjlist = (int**)malloc(sizeof(int*)*nVert);
   nEdges = (int*)malloc(sizeof(int)*nVert);

   //Reading the file again
   pFile = fopen(fname , "r");
   if (pFile == NULL) perror ("Error opening file");
   else
   {
      k = 0;
      while(fgets (mystring , 100 , pFile) != NULL )
      {
         //Determining number of spaces
         l = strlen(mystring);
         spaces = 0;
         for(j = 0; j < l; j++)
         {
            if(mystring[j] == ' ') spaces++;
         }

         nEdges[k] = spaces;
         //Allocating appropriate memory
         adjlist[k] = (int*)malloc(sizeof(int)*(spaces+1));
         pEnd = mystring;
         i = 0;
         while(i < spaces+1)
         {
            adjlist[k][i] = strtol(pEnd, &pEnd, 10);
            i++;
         }
         k++;
      }
     fclose (pFile);
   }
   return 0;
}


void printAdjList()
{
int i,j;
for(i = 0; i < nVert; i++)
{
   for(j = 0; j < nEdges[i] + 1; j++)
   {
      printf("%d ", adjlist[i][j]);
   }
   printf("\n");
}
}


void initializeNallmem()
{
int i;
//Allocating memory for vertices of type vertex data structure
vertices = (Pvertex*)malloc(sizeof(Pvertex)*nVert);

//Initialize vertex nodes
for(i = 0; i < nVert; i++)
{
   vertices[i] = (Pvertex)malloc(sizeof(vertex));
   vertices[i]->node = i + 1; //Node numbering starts from 1 
   vertices[i]->num = 0;
   vertices[i]->low = LARGE_NEG_NUMB;
   vertices[i]->color = WHITE;
}
}

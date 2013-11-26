//Graph biconnectivity 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "biconn.h"

static int nVert = 0;
static int **adjlist; //Adjacency list representation of input vertices
static int *nEdges;

int main(int argc, char **argv)
{

FILE *input;
char fname[] = "1.in";

read_file(fname);
printAdjList();

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

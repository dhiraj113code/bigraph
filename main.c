//Graph biconnectivity 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "biconn.h"


static int **adjlist; //Adjacency list representation of input vertices

int main(int argc, char **argv)
{

FILE *input;
int **adjlist; //Adjlist representation of the input graph
char fname[] = "1.in";

read_file(fname);

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
   printf("debug_info : Number of lines = %d\n", lines);
   fclose(pFile);

   //Allocating memory for integers array pointers
   //adjlist = *Padjlist;
   adjlist = (int**)malloc(sizeof(int*)*lines);

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
         //printf("spaces = %d\n", spaces);

         //Allocating appropriate memory
         adjlist[k] = (int*)malloc(sizeof(int)*(spaces+1));
         pEnd = mystring;
         i = 0;
         while(i < spaces+1)
         {
            //printf("pend = %s\n", pEnd);
            //printf("i = %d\n", i);
            adjlist[k][i] = strtol(pEnd, &pEnd, 10);
            printf("%d\n", adjlist[k][i]);
            i++;
         }
         printf("%s", mystring);
         k++;
      }
     fclose (pFile);
   }
   return 0;
}

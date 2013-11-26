//Graph biconnectivity 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#define FILE_NAME_SIZE 100
#define DEBUG FALSE 
#define MORESTATS FALSE

static int nVert = 0;
static int **adjlist; //Adjacency list representation of input vertices
static int *nEdges;
static Pvertex *vertices;
static int *artPoints;
static int *vertexStack;
static int **biconnComps;
static int *bcLength;
static int Ncalls = 0;
static int Gnum = 0;
static int artIndex = 0;
static int stackEnd;
static int bcI = 0;
static FILE *logfile;
static FILE *stackfile;

int main(int argc, char **argv)
{
/* initialize random seed: */
  srand (time(NULL));

FILE *input;
if(argc !=2) {printf("Wrong number of input arguments specified\n"); exit(-1);}
char *fname;
fname = (char*)malloc(sizeof(char)*FILE_NAME_SIZE);
fname = argv[1];
read_file(fname);
if(DEBUG && MORESTATS) printAdjList();

if(DEBUG) logfile = fopen("debug.log", "w");
if(DEBUG) stackfile = fopen("stack.log", "w");
initializeNallmem();

int start_index = rand() % nVert;
printf("start_node = %d\n", start_index + 1);
biconn(vertices[start_index]->node, DUMMY_PARENT);
printArtPoints();
printBiconnComponents();
if(DEBUG) fclose(logfile);
if(DEBUG) fclose(stackfile);
}



void biconn(int node, int parent)
{
  if(DEBUG) printStack(FALSE);
  if(DEBUG) printVertices();
  Ncalls++;
  
  int i, index, test_node, test_node_index, node_color, new_parent, parent_index, parent_stackPos;
  index = node - 1;
  parent_index = parent - 1;

  Gnum++;
  vertices[index]->color = GRAY;
  vertices[index]->num = Gnum;
  vertices[index]->low = vertices[index]->num;
  new_parent = node;

  //Push it onto the vertex stack
  vertexStack[stackEnd] = node;
  vertices[index]->stackPos = stackEnd;
  stackEnd++; 
  
  //Loop over the vertices in the adjacency list for this node
  for(i = 1; i < nEdges[index] + 1; i++)
  {
     test_node = adjlist[index][i];
     test_node_index = test_node - 1;
     node_color = vertices[test_node_index]->color;
     if(node_color == WHITE)
     {
        biconn(test_node, new_parent);
     }
     else if(node_color == GRAY && test_node != parent) //Back edge to the parent
     {
        //Lesser but now parent, pass on the goodness
        vertices[index]->low = getLow(vertices[index]->low , vertices[test_node_index]->low);
     }
  }
  //Backtracking to parent
  if(parent != DUMMY_PARENT)
  {
     if(vertices[index]->low < vertices[parent_index]->low )
     {
        vertices[parent_index]->low = vertices[index]->low;
     }
     else //Break the bond
     {
        //Parent is an articulation point
        artPoints[artIndex] = parent;
        artIndex++;

        //Peel off the vertices to the right of the articulation point in the stack
        //To get the biconnected components
        parent_stackPos = vertices[parent_index]->stackPos;
        bcLength[bcI] = stackEnd - parent_stackPos; 
        biconnComps[bcI] = (int*)malloc(sizeof(int)*bcLength[bcI]);
     
        if(DEBUG) printStack(TRUE); 
        i = 0;
        while(stackEnd > parent_stackPos) //peel elements to the right in the stack array
        {
           biconnComps[bcI][i] = vertexStack[stackEnd-1];
           stackEnd--;
           i++;
        }
        stackEnd++;
        if(DEBUG) printStack(TRUE);
        bcI++;
     }
  }
  //When all nodes reachable from a particular node are reached, color the node black
  vertices[index]->color = BLACK;
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

//Allocating maximing possible memory for articulation points
artPoints = (int*)malloc(sizeof(int)*nVert);

vertexStack = (int*)malloc(sizeof(int)*nVert);
stackEnd = 0;

//Allocating memory for biconnected components
biconnComps = (int**)malloc(sizeof(int*)*nVert);
bcLength = (int*)malloc(sizeof(int)*nVert);
}


void printVertices()
{
int i;
int node, num, low, color;
fprintf(logfile, "----------------------------%d------------------------------\n", Ncalls);
for(i = 0; i < nVert; i++)
{
   node = vertices[i]->node;
   num = vertices[i]->num;
   low = vertices[i]->low;
   color = vertices[i]->color;
   fprintf(logfile, "node = %d, num = %d, low =  %d, color = %c\n", node, num, low, colorSymbol(color));
}
fprintf(logfile, "------------------------------------------------------------\n");
}

char colorSymbol(int color_value)
{
char c;
switch(color_value)
{
   case WHITE:
      return 'W';
      break;
   case GRAY:
      return 'G';
      break;
   case BLACK:
      return 'B';
      break;
   default:
      printf("error_info : Unknown color\n");
      exit(-1);
}
}

int getLow(int a, int b)
{
   if( a <= b)
      return a;
   else
      return b;
}

void printArtPoints()
{
   int i;
   printf("Articulation points\n");
   printf("----------------------------------------------------------\n");
   for(i = 0; i < artIndex; i++)
   {
      printf("%d ", artPoints[i]);
   }
   printf("\n");
   printf("----------------------------------------------------------\n");
}


void printBiconnComponents()
{
int i,j;
printf("Biconnected component vertices\n");
printf("----------------------------------------------------------\n");
for(i = 0; i < bcI; i++)
{
   for(j = 0; j < bcLength[i]; j++)
   {
      printf("%d ", biconnComps[i][j]);
   }
   printf("\n");
}
printf("----------------------------------------------------------\n");
}


void printStack(int isPeel)
{
int i;
if(isPeel) fprintf(stackfile, "----------------------------------------------------------\n");
for(i = 0; i < stackEnd; i++)
   fprintf(stackfile, "%d ", vertexStack[i]);
fprintf(stackfile, "\n");
if(isPeel) fprintf(stackfile, "----------------------------------------------------------\n");
}

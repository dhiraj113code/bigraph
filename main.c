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
static FILE *logfile, *stackfile, *edgelog;

//Edge stack used for computign biconnected components
static totalEdges = 0;
static Pedge *edgeStack;
static int eS_right = 0;
static int rootNode;

//Charging
static int cv = 0;
static int ce = 0;

int main(int argc, char **argv)
{
/* initialize random seed: */
srand (time(NULL));
int start_index;
FILE *input;
if(argc < 2) {printf("Wrong number of input arguments specified specify the file name\n"); exit(-1);}
char *fname;
fname = (char*)malloc(sizeof(char)*FILE_NAME_SIZE);
fname = argv[1];

//Reading the input file and storing the vertices in adjlist
read_file(fname);

if(DEBUG && MORESTATS) printAdjList();
if(DEBUG) logfile = fopen("debug.log", "w");
if(DEBUG) stackfile = fopen("stack.log", "w");
if(DEBUG)  edgelog = fopen("edge.log", "w");

//All memory allocation and other initialization stuff done here
initializeNallmem();

//Choosing a random start node to start the DFS
start_index = rand() % nVert; //The start node is determined randomnly
rootNode = vertices[start_index]->node;

//Call the the recursive biconnectivity function
//**************************************************************
biconn(rootNode, DUMMY_PARENT);
//**************************************************************

printf("start_node = %d\n", start_index + 1);
printArtPoints();
printBiconnVertices();

/*--------------------------------------------*/
if(DEBUG) fclose(logfile);
if(DEBUG) fclose(stackfile);
if(DEBUG) fclose(edgelog);

printf("Number of Vertices = %d\n", nVert);
printf("Number of Edges = %d\n", totalEdges);
printf("Unary operations charged to vertices = %d\n", cv);
printf("Unary operations charged to edges = %d\n", ce);
printf("Total number of Unary operations charge = %d\n", cv + ce);
}



void biconn(int node, int parent)
{
  Ncalls++;
  if(DEBUG) printStack(FALSE);
  if(DEBUG) printVertices();

  //eS_left is local
  int eS_left = eS_right;   cv++; 
  if(DEBUG) printEdgeStack(TREE_EDGE, eS_left);
  
  int i, index, test_node, test_node_index, node_color, new_parent, parent_index;
  index = node - 1;   cv++;
  parent_index = parent - 1;   cv++;

  vertices[index]->color = GRAY;   cv++;
  vertices[index]->num = Gnum;   cv++;
  vertices[index]->low = vertices[index]->num;   cv++;
  new_parent = node;   cv++;
  Gnum++;   cv++;

  //Push it onto the vertex stack
  vertexStack[stackEnd] = node;   cv++;
  vertices[index]->stackPos = stackEnd;   cv++;
  stackEnd++;   cv++;
  
  //Loop over the vertices in the adjacency list for this node
  for(i = 1; i < nEdges[index] + 1; i++)
  {
     ce += 2;
     test_node = adjlist[index][i];   ce++;
     test_node_index = test_node - 1;   ce++;
     node_color = vertices[test_node_index]->color;   ce++; ce++;
     if(node_color == WHITE) //forward edge
     {
        edgeStack[eS_right]->tail = node;   ce++;
        edgeStack[eS_right]->head = test_node;   ce++;
        eS_right++;   ce++;
        biconn(test_node, new_parent);  ce++;
     }
     else if(node_color == GRAY && test_node != parent) //Back edge not to the parent
     {
        ce++;
        if(vertices[test_node_index]->num < vertices[index]->num) //The vertex was visited before the current vertex
        {
           ce++;
           //Lesser but not parent, pass on the goodness
           vertices[index]->low = getLow(vertices[index]->low , vertices[test_node_index]->num);   ce++;

           //Push edge on to the stack
           edgeStack[eS_right]->tail = node;   ce++;
           edgeStack[eS_right]->head = test_node;   ce++;
           eS_right++;   ce++;
           if(DEBUG) printEdgeStack(FORWARD_EDGE, eS_left);
        }
        else
        {
           printf("debug_info : I deduce that this will never occur\n");
        }
     }
  }
  //Backtracking to parent
  if(parent != DUMMY_PARENT)
  {
     vertices[parent_index]->low = getLow(vertices[parent_index]->low, vertices[index]->low);  cv++;
     if(vertices[index]->low < vertices[parent_index]->num )
     {
        cv++;
        //Move back the current element stack pointer
        if(DEBUG) printEdgeStack(BACKWARD_EDGE, eS_left);
     }
     else //Break the bond
     {
        cv += 2;
        //Parent is an articulation point
        //Search if it is already present in the articulation points list. Else add the vertex.
        artPoints[artIndex] = parent;   cv++;
        artIndex++;   cv++;

        //Strip out the edges corresponding to this articulatio point
        if(DEBUG) printBiconn(eS_left-1, eS_right);

        //Add biconnected components vertices.
        //Search the array to check if the vertex is already there. Else add the vertex
        storeBiconnVerts(eS_left-1, eS_right);   cv++;

        eS_right = eS_left-1;   cv++;
        if(DEBUG) printEdgeStack(ARTICULATE_EDGE, eS_left);
     }
     //When all nodes reachable from a particular node are reached, color the node black
     vertices[index]->color = BLACK;   cv++;
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
   if(pFile == NULL) {printf("error_info : Specified file not found\n"); exit(-1);}
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
         totalEdges += spaces;
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

//Allocating memory of edgeStack
edgeStack = (Pedge*)malloc(sizeof(Pedge)*totalEdges);
for(i = 0; i < totalEdges; i++)
{
   edgeStack[i] = (Pedge)malloc(sizeof(edge));
}
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
   //Remove duplicates if any and print
   //Create a duplicate free array
   int local_artPoints[artIndex-1];
   int local_artsize = 0;
   int i, j;
   int artvertex;
   for(i = 0; i < artIndex-1; i++)
   {
      if(!(simpleSearch(artPoints[i], local_artPoints, local_artsize)))
      {
         local_artPoints[local_artsize] = artPoints[i];
         local_artsize++;
      }
   }
   
   qsort (local_artPoints, local_artsize, sizeof(int), compare);
   printf("Articulation points\n");
   printf("----------------------------------------------------------\n");
   for(i = 0; i < local_artsize; i++)
   {
      printf("%d ", local_artPoints[i]);
   }
   printf("\n");
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


void printEdgeStack(int edgeType, int left)
{
int i;
char c;
switch(edgeType)
{
   case TREE_EDGE:
      c = 'T';
      break;
   case FORWARD_EDGE:
      c = 'F';
      break;
   case BACKWARD_EDGE:
      c = 'B';
      break;
   case ARTICULATE_EDGE:
      c = 'A';
      break;
   default:
      printf("error_info : Unknown edge type\n");
      exit(-1);
}

fprintf(edgelog, "(%c)l=%d, r =%d : ", c, left, eS_right);
for(i = 0; i < left; i++)
   fprintf(edgelog, "(%d,%d),  ", edgeStack[i]->tail, edgeStack[i]->head);
fprintf(edgelog, "|||");
for(i = left; i < eS_right; i++)
   fprintf(edgelog, "(%d,%d),  ", edgeStack[i]->tail, edgeStack[i]->head);
fprintf(edgelog, "\n");
}


void printBiconn(int left, int right)
{
   int i;
   for(i = left; i < right; i++)
   {
      printf("(%d,%d) ", edgeStack[i]->tail, edgeStack[i]->head);
   }
   printf("\n");
}


void storeBiconnVerts(int left, int right)
{
   int i;
   int l = 0;
   int size= right - left;   cv++;
   biconnComps[bcI] = (int*)malloc(sizeof(int)*2*size); //Allocating the maximum possible memory for this
      
   for(i = left; i < right; i++)
   {
      cv += 2;
      //Check if the node is already present. If not add at the node to the biconn vertices list
      if(!(simpleSearch(edgeStack[i]->tail, biconnComps[bcI], l)))
      {
         biconnComps[bcI][l] = edgeStack[i]->tail;   cv++;
         l++;   cv++;
      }
      if(!(simpleSearch(edgeStack[i]->head, biconnComps[bcI], l)))
      {
         biconnComps[bcI][l] = edgeStack[i]->head;   cv++;
         l++;   cv++;
      }
   }
   bcLength[bcI] = l;   cv++;
   bcI++;   cv++;
}


int simpleSearch(int value, int *A, int n)
{
int i;
for(i = 0; i < n; i++)
{
   if(A[i] == value)
      return TRUE;
}
return FALSE;
}


void printBiconnVertices()
{
int k,i,ksort;
Pele *elements;
elements = (Pele*)malloc(sizeof(Pele)*bcI);
for(k = 0; k < bcI; k++)
   elements[k] = (Pele)malloc(sizeof(ele));

//Sorting each biconnected components vertex lines

for(k = 0; k < bcI; k++)
{
   qsort (biconnComps[k], bcLength[k], sizeof(int), compare);
   //Creating an array of the first elements of the each line
   elements[k]->node = biconnComps[k][0];
   elements[k]->index = k;
   elements[k]->node2 = biconnComps[k][1];
}

qsort(elements, bcI, sizeof(Pele), compare2); //sorting the array of first elements

printf("----------------------------------------------------------------------------------------\n");
printf("Biconnected components\n");
printf("----------------------------------------------------------------------------------------\n");
for(k = 0; k < bcI; k++)
{
   ksort = elements[k]->index;
   for(i = 0; i < bcLength[ksort]; i++)
   {
      printf("%d ", biconnComps[ksort][i]); 
   }
   printf("\n");
}
printf("----------------------------------------------------------------------------------------\n");
printf("Articulation bridges\n");
printf("----------------------------------------------------------------------------------------\n");
for(k = 0; k < bcI; k++)
{
   ksort = elements[k]->index;
   if(bcLength[ksort] == 2)
   {
      for(i = 0; i < bcLength[ksort]; i++)
      {
         printf("%d ", biconnComps[ksort][i]);
      }
      printf("\n");
   }
}
}


int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}


int compare2 (const void * a, const void * b)
{
  int comp = (*(Pele*)a)->node - (*(Pele*)b)->node;
  int comp2;
  if(comp > 0)
     return 1;
  else if(comp == 0)
  {
     comp2 = (*(Pele*)a)->node2 - (*(Pele*)b)->node2;
     if(comp2 >0)
        return 1;
     else if(comp2 == 0)
        return 0;
     else
        return -1;
  }
  else
     return -1;
}


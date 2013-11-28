//graph biconnectivity
#define TRUE 1
#define FALSE 0

#define MAX_GRAPH_SIZE 100

#define LARGE_NEG_NUMB -1000

#define WHITE 0
#define GRAY 1
#define BLACK 2

#define DUMMY_PARENT -1

#define TREE_EDGE 0
#define FORWARD_EDGE 1
#define BACKWARD_EDGE 2
#define ARTICULATE_EDGE 3

typedef struct vertex_ {
   unsigned node;
   unsigned num;
   unsigned low;
   unsigned color;
   unsigned stackPos;
} vertex, *Pvertex;


typedef struct edge_ {
   unsigned tail;
   unsigned head;
} edge, *Pedge;


typedef struct ele_{
   unsigned node;
   unsigned index;
   unsigned node2;
} ele, *Pele;


void biconn(int node, int parent);

int read_file(char fname[]);
void printAdjList();
void initializeNallmem();
void printVertices();
char colorSymbol(int color_value);
int getLow(int a, int b);
void printArtPoints();
void printStack();
void printEdgeStack(int edgeType, int left);
void printBiconn(int left, int right);
int simpleSearch(int value, int *A, int n);
void storeBiconnVerts(int left, int right);
void globalPrint();
void simpleSort();
int compare (const void * a, const void * b);
int compare2 (const void * a, const void * b);

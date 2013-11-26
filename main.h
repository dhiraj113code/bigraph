//graph biconnectivity
#define TRUE 1
#define FALSE 0

#define MAX_GRAPH_SIZE 100

#define LARGE_NEG_NUMB -1000

#define WHITE 0
#define GRAY 1
#define BLACK 2

typedef struct vertex_ {
   unsigned node;
   unsigned num;
   unsigned low;
   unsigned color;
} vertex, *Pvertex;


void biconn(int node);

int read_file(char fname[]);
void printAdjList();
void initializeNallmem();
void printVertices();
char colorSymbol(int color_value);

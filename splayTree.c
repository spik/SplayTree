/*Splay trees:

A splay tree is a self-adjusting binary search tree with the additional property that recently accessed elements are quick to access again.
The tree is reorganized after every lookup, as opposed to an ordinary BST, in order to keep the element accessed most often in the root. This means that elements that are often accessed
are found in the top of the tree.

splaying the tree for a certain element rearranges the tree so that the element is placed at the root of the tree
Frequently accessed elements are moved nearer the root where they can be accessed again more quickly
Splay trees do not need to store any bookkeeping data

Disadvantages:
the height can be linear, which happens for example after accessing all n elements in a non-decreasing order.

Operations:
when a node x is accessed, a splay operation is performed on x to move it to the root, by carrying out a sequence of splay steps, each of which moves x closer to the root.
Each step depends on three factors:
- whether x is the left or right child of its parent node, p
- whether p is the root or not, and of not
- whether p is the left or right child of its parent, g (the grandparent of x).

After any splay operations, the great-grandparent of x (gg) is set to point to x.
If gg is null, x is the root and must be updated as such.

zig step:
This step is performed when p is the root. This rotates x to the root, p to the right of x and the right child of x to the left child of p.
Condition: When the last accessed node (node x) is a child of the root
operation: single left or right rootation.

zig-zig step:
This step is done when p is not the root and x and p are either both right children or are both left childrenof the root. This places x in the root, p as right child of x and g as a right child of p,
as well as rootating the right child of x to the left child of p and the right child of p to the left side of g.
Condition: When X is the left child of the parent P and P is the left child of the grandparent or if they are both right parents
Operation:

zig-zag step:
This step is performed when p is not the root and x is a right child and p is a left child or vice versa. This Places x in the root, g as a right child to x and p as a left child to x.
The left child of x is rotated to the right child of p, and the right child of x is rotated to the left child of g.
Condition: When X is the left child of P and P is the right child of G or vice versa
Operation: Double rootation

Retrieval:
Find node X using BST methods, then splay X up tp the root

Insertion:
Insert node X using BST methods, then splay X up to the root

How to run:
compile: gcc splayTree.c -o splayTree
run: ./splayTree
*/
#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#include<limits.h>

//#define DEBUG
#ifdef DEBUG
    #define NUM_ITERATIONS 10
#else
    #define NUM_ITERATIONS 100
#endif

struct node{
    double timestamp;
    struct node *left;
    struct node *right;
};

void print_ascii_tree(struct node * t);

/* thresholds for timestamp. */
double l_threshold, h_threshold;
/* seed value for timestamp generation */
long seed;

struct node* new_node(double timestamp){
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    new_node->timestamp = timestamp;
    new_node->left = NULL;
    new_node->right = NULL;
    return (new_node);
}

struct node *right_rotate(struct node *x){
    struct node *y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

struct node *left_rotate(struct node *x){
    struct node *y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

struct node *splay(struct node *root, double timestamp){

    //base case as this is a reqursive function
    if(root == NULL || root->timestamp == timestamp) return root;

    if(root->timestamp > timestamp){
        if (root->left == NULL) {
            return root;
        }
        //zig-zig
        if (root->left->timestamp > timestamp) {

            //recursively call splay to bring the timestamp as root of left left
            root->left->left = splay(root->left->left, timestamp);

            //rotate right the first time
            root = right_rotate(root);
        }
        //zig-zag
        else if (root->left->timestamp < timestamp) {
            root->left->right = splay(root->left->right, timestamp);
            if (root->left->right != NULL){
                root->left = left_rotate(root->left);
            }
        }
        return (root->left == NULL)? root: right_rotate(root);
    }
    else /*if(root->timestamp < timestamp)*/{
        if (root->right == NULL){
            return root;
        }

        //zig-zag
        if (root->right->timestamp > timestamp) {
            root->right->left = splay(root->right->left, timestamp);
            if (root->right->left != NULL){
                root->right = right_rotate(root->right);
            }
        }
        //zig-zig
        else if(root->right->timestamp < timestamp){
            root->right->right = splay(root->right->right, timestamp);
            root = left_rotate(root);
        }
        return (root->right == NULL)? root: left_rotate(root);
    }
}

struct node *insert(struct node *root, double timestamp){

    //If tree is empty, just create the new node
    if (root == NULL) {
        return new_node(timestamp);
    }
    //Splay the current node to the root
    root = splay(root, timestamp);

    struct node *node = new_node(timestamp);

    //If the timestamp is less than the root timestamp. In this case the old root should be the right child of the new node
    //Insert new node at root
    if (root->timestamp > timestamp) {

        //Copy the left chiold of the root to the new node
        node->left = root->left;

        //Make the root the right child if the new node
        node->right = root;

        //The root no longer has a left child
        root->left = NULL;
    }
    //If the timestamp is greater than the root, the root should be the left child of the new node
    // Insert new node at root
    else if(root->timestamp < timestamp){

        //Make the right child of the root the right child of the new node
        node->right = root->right;

        //Set the root to the left child of the new node
        node->left = root;

        //The root no longer has a right child
        root->right = NULL;
    }
    //root->timestamp == timestamp
    else{
        return root;
    }
    //the new node becomes the new root
    return node;
}

//Deletes the node with the specified timestamp
struct node *remove_node(struct node *root, double timestamp){

    //The tree is empty, just return
    if (root == NULL) return root;

    //Splay the node we want to delete to the root
    root = splay(root, timestamp);

    if (root->timestamp == timestamp) {

        //If the root doesn't have any left children
        if (root->left == NULL) {

            //Update the root to the right child of the root. This way the new root,
            //i.e. the node we want to delete that was splayed to the root, is deleted.
            root = root->right;
        }
        //Otherwise
        else{

            //Store the right child of the root in a temporary node
            struct node *temp = root->right;

            //Set the root to the node's left child. Here the root is deleted.
            root = root->left;

            //Splay the root to the top
            root = splay(root, timestamp);

            //Set the right child of the new node to the right child of the old root
            root->right = temp;
        }
    }
    return root;
    //else: it wasn't in the tree to remove
}

/*find the node with the highest priority, i.e. the node with the smalles timestamo*/
struct node *find_min(struct node *root){

    //The tree is empty, return NULL
    if (root == NULL){
        return NULL;
    }
    //The root has no left children, which means that the root has the smallest timestamp
    else if (root->left == NULL){
        return root;
    }
    else{
        //Recursively call find_min to get to the node to the farthest left, i.e. the node with the smallest timestamp
        return find_min(root->left);
    }
}

/*	timer */
double read_timer() {

  static bool initialized = false;
  static struct timeval start;
  struct timeval end;
  if( !initialized )
  {
	  gettimeofday( &start, NULL );
	  initialized = true;
  }
  gettimeofday( &end, NULL );

  return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);

}

double generate_timestamp(){

	double timestamp;

	/* uniform distribution */
	timestamp = h_threshold * drand48();

	return timestamp;
}

/* generates a splay tree with the number of nodes spcified in size*/
struct node *generate(struct node *root, int size){

	for(int i = 0; i < size; i++){

    	double t = l_threshold + generate_timestamp();//drand48() * h_threshold;

    	root = insert(root, t);
	}
    return root;
}

/* Simulates a usage of the splay tree where one element
   is removed and another is inserted for a number of iterations.
   The average time taken for an insert + remove operation
   is returned */
double simulate(struct node *root){

	double t_start, t_end, timeElapsed;
	int i;
	t_start = read_timer();
	for(i = 0; i < NUM_ITERATIONS; i++){

        /* dequeue element with the highest priority */
		root = remove_node(root, find_min(root)->timestamp);

        #ifdef DEBUG
            printf("After remove\n");
            print_ascii_tree(root);
            printf("\n\n");
        #endif

		/* enqueue an element with a randomly generated timestamp */
		double t = l_threshold + generate_timestamp();//drand48() * h_threshold;

        root = insert(root, t);

        #ifdef DEBUG
            printf("After insert\n");
            print_ascii_tree(root);
            printf("\n\n");
        #endif
	}


#ifdef DEBUG
    printf("Last print\n");
    print_ascii_tree(root);
    printf("\n\n");
#endif
	t_end = read_timer();
	timeElapsed = (t_end - t_start) / NUM_ITERATIONS;

	return timeElapsed;
}

/***************************************************************************************************************
The following methods (not including main) was taken from web.archive.org from an article originally posted on
openasthra.com. The code was written by user "ponnada" and posted on December 21,2007 at 8:14 pm. The full
code can be found here:
http://web.archive.org/web/20071224095835/http://www.openasthra.com/wp-content/uploads/2007/12/binary_trees1.c
****************************************************************************************************************/
typedef struct asciinode_struct asciinode;

struct asciinode_struct
{
  asciinode * left, * right;

  //length of the edge from this node to its children
  int edge_length;

  int height;

  int lablen;

  //-1=I am left, 0=I am root, 1=right
  int parent_dir;

  //max supported unit32 in dec, 10 digits max
  char label[11];
};


#define MAX_HEIGHT 1000
int lprofile[MAX_HEIGHT];
int rprofile[MAX_HEIGHT];
#define INFINITY (1<<20)

//adjust gap between left and right nodes
int gap = 3;

//used for printing next node in the same level,
//this is the x coordinate of the next char printed
int print_next;

int MIN (int X, int Y)
{
  return ((X) < (Y)) ? (X) : (Y);
}

int MAX (int X, int Y)
{
  return ((X) > (Y)) ? (X) : (Y);
}

asciinode * build_ascii_tree_recursive(struct node * t)
{
  asciinode * node;

  if (t == NULL) return NULL;

  node = malloc(sizeof(asciinode));
  node->left = build_ascii_tree_recursive(t->left);
  node->right = build_ascii_tree_recursive(t->right);

  if (node->left != NULL)
  {
    node->left->parent_dir = -1;
  }

  if (node->right != NULL)
  {
    node->right->parent_dir = 1;
  }

  sprintf(node->label, "%g", t->timestamp);
  node->lablen = strlen(node->label);

  return node;
}


//Copy the tree into the ascii node structre
asciinode * build_ascii_tree(struct node * t)
{
  asciinode *node;
  if (t == NULL) return NULL;
  node = build_ascii_tree_recursive(t);
  node->parent_dir = 0;
  return node;
}

//Free all the nodes of the given tree
void free_ascii_tree(asciinode *node)
{
  if (node == NULL) return;
  free_ascii_tree(node->left);
  free_ascii_tree(node->right);
  free(node);
}

//The following function fills in the lprofile array for the given tree.
//It assumes that the center of the label of the root of this tree
//is located at a position (x,y).  It assumes that the edge_length
//fields have been computed for this tree.
void compute_lprofile(asciinode *node, int x, int y)
{
  int i, isleft;
  if (node == NULL) return;
  isleft = (node->parent_dir == -1);
  lprofile[y] = MIN(lprofile[y], x-((node->lablen-isleft)/2));
  if (node->left != NULL)
  {
	  for (i=1; i <= node->edge_length && y+i < MAX_HEIGHT; i++)
    {
	    lprofile[y+i] = MIN(lprofile[y+i], x-i);
    }
  }
  compute_lprofile(node->left, x-node->edge_length-1, y+node->edge_length+1);
  compute_lprofile(node->right, x+node->edge_length+1, y+node->edge_length+1);
}

void compute_rprofile(asciinode *node, int x, int y)
{
  int i, notleft;
  if (node == NULL) return;
  notleft = (node->parent_dir != -1);
  rprofile[y] = MAX(rprofile[y], x+((node->lablen-notleft)/2));
  if (node->right != NULL)
  {
	  for (i=1; i <= node->edge_length && y+i < MAX_HEIGHT; i++)
    {
	    rprofile[y+i] = MAX(rprofile[y+i], x+i);
    }
  }
  compute_rprofile(node->left, x-node->edge_length-1, y+node->edge_length+1);
  compute_rprofile(node->right, x+node->edge_length+1, y+node->edge_length+1);
}

//This function fills in the edge_length and
//height fields of the specified tree
void compute_edge_lengths(asciinode *node)
{
  int h, hmin, i, delta;
  if (node == NULL) return;
  compute_edge_lengths(node->left);
  compute_edge_lengths(node->right);

  /* first fill in the edge_length of node */
  if (node->right == NULL && node->left == NULL)
  {
	  node->edge_length = 0;
  }
  else
  {
    if (node->left != NULL)
    {
	    for (i=0; i<node->left->height && i < MAX_HEIGHT; i++)
      {
		    rprofile[i] = -INFINITY;
	    }
	    compute_rprofile(node->left, 0, 0);
	    hmin = node->left->height;
    }
    else
    {
	    hmin = 0;
    }
	  if (node->right != NULL)
    {
	    for (i=0; i<node->right->height && i < MAX_HEIGHT; i++)
      {
		    lprofile[i] = INFINITY;
	    }
	    compute_lprofile(node->right, 0, 0);
	    hmin = MIN(node->right->height, hmin);
    }
    else
    {
	    hmin = 0;
    }
	  delta = 4;
	  for (i=0; i<hmin; i++)
    {
	    delta = MAX(delta, gap + 1 + rprofile[i] - lprofile[i]);
    }

    //If the node has two children of height 1, then we allow the
    //two leaves to be within 1, instead of 2
	  if (((node->left != NULL && node->left->height == 1) ||
	      (node->right != NULL && node->right->height == 1))&&delta>4)
    {
      delta--;
    }

    node->edge_length = ((delta+1)/2) - 1;
  }

  //now fill in the height of node
  h = 1;
  if (node->left != NULL)
  {
	  h = MAX(node->left->height + node->edge_length + 1, h);
  }
  if (node->right != NULL)
  {
	  h = MAX(node->right->height + node->edge_length + 1, h);
  }
  node->height = h;
}

//This function prints the given level of the given tree, assuming
//that the node has the given x cordinate.
void print_level(asciinode *node, int x, int level)
{
  int i, isleft;
  if (node == NULL) return;
  isleft = (node->parent_dir == -1);
  if (level == 0)
  {
	  for (i=0; i<(x-print_next-((node->lablen-isleft)/2)); i++)
    {
	    printf(" ");
    }
	  print_next += i;
	  printf("%s", node->label);
	  print_next += node->lablen;
  }
  else if (node->edge_length >= level)
  {
	  if (node->left != NULL)
    {
	    for (i=0; i<(x-print_next-(level)); i++)
      {
		    printf(" ");
	    }
	    print_next += i;
	    printf("/");
	    print_next++;
    }
	  if (node->right != NULL)
    {
	    for (i=0; i<(x-print_next+(level)); i++)
      {
		    printf(" ");
	    }
	    print_next += i;
	    printf("\\");
	    print_next++;
    }
  }
  else
  {
	  print_level(node->left,
                x-node->edge_length-1,
                level-node->edge_length-1);
	  print_level(node->right,
                x+node->edge_length+1,
                level-node->edge_length-1);
  }
}

//prints ascii tree for given Tree structure
void print_ascii_tree(struct node * t)
{
  asciinode *proot;
  int xmin, i;
  if (t == NULL) return;
  proot = build_ascii_tree(t);
  compute_edge_lengths(proot);
  for (i=0; i<proot->height && i < MAX_HEIGHT; i++)
  {
	  lprofile[i] = INFINITY;
  }
  compute_lprofile(proot, 0, 0);
  xmin = 0;
  for (i = 0; i < proot->height && i < MAX_HEIGHT; i++)
  {
	  xmin = MIN(xmin, lprofile[i]);
  }
  for (i = 0; i < proot->height; i++)
  {
	  print_next = 0;
	  print_level(proot, -xmin, i);
	  printf("\n");
  }
  if (proot->height >= MAX_HEIGHT)
  {
	  printf("(This tree is taller than %d, and may be drawn incorrectly.)\n", MAX_HEIGHT);
  }
  free_ascii_tree(proot);
}

/* Driver program to test above function*/
/*int main()
{
    struct node *root = new_node(100);
    root = insert(root, 50);
    root = insert(root, 200);
    root = insert(root, 40);
    root = insert(root, 30);
    root = insert(root, 20);
    root = insert(root, 25);
    printf("Preorder traversal of the modified Splay tree is \n");
    preOrder(root);
    printf("\n");

    remove_node(root, 25);
    printf("Preorder traversal of the modified Splay tree is \n");
    preOrder(root);
    printf("\n");
    return 0;
}*/
int main (int argc, char *argv[]){

	int size = (argc > 1)? atoi(argv[1]) : 1000;
	seed = (argc > 2)? atol(argv[2]) : 831970590;

    struct node *root = NULL;

	srand48(seed);

	l_threshold = 1;
	h_threshold = size;

	root = generate(root, size);
    #ifdef DEBUG
        printf("First print\n" );
        print_ascii_tree(root);
    #endif
    printf("\n");

	double timeElapsed = simulate(root);

	printf("time elapsed: %g\n", timeElapsed);
	//printf("\t%d\t%g\n", size, timeElapsed);

    printf("size: %lu\n", sizeof(struct node)*size);

	return 0;
}

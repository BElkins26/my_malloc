//replace malloc here with the appropriate version of mymalloc
#define MALLOC(n) my_malloc(n)
//replace free here with the appropriate version of myfree
#define FREE(n) my_free(n)
//define DUMP_HEAP() to be the dump_heap() function that you write
#define DUMP_HEAP() dump_heap()
struct Block {
  int occ;              // whether block is occupied
  int size;             // size of block (including header)
  struct Block *prev;   // pointer to previous block
  struct Block *next;   // pointer to next block
};
void *my_malloc(int size);
void my_free(void *data);
void dump_heap();

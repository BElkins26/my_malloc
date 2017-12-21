#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include "custom_malloc.h"

 

static struct Block *head = NULL;       // head of list
void check_coalesce(struct Block *pointer);



void *my_malloc(int size)
{
	struct Block *current;
	struct Block *temp;
	int smallest= -1;
	struct Block *smallestblock;
	if(head == NULL)
	{
		current=sbrk(sizeof(struct Block)+size);
		current->occ =1;
		current->size=size+sizeof(struct Block);
		current->prev = current->next = NULL;
		head=current;
	}
	else
	{
		
		for(temp = head; temp->next != NULL; temp=temp->next)
		{
			if(temp->occ == 0) {			
				if((temp->size-sizeof(struct Block)) == size)
				{
					smallestblock=temp;
					smallestblock->occ=1;
					return smallestblock+1;
					break;
				}
				else if(smallest == -1)
				{
					if((temp->size-sizeof(struct Block))-size > 0)
					{
						smallest = temp->size-sizeof(struct Block)-size;
						smallestblock=temp;
					}
				}
				else if(((temp->size-sizeof(struct Block))-size < smallest) && ((temp->size-sizeof(struct Block))-size) > 0)
				{
					smallest=temp->size-sizeof(struct Block)-size;
					smallestblock=temp;
				}
			}
		}
	if(smallest != -1)
	{
		current = smallestblock;
		current->occ =1;
		if(smallest > sizeof(struct Block))
		{
			struct Block *new = (char *)current + (current->size-smallest);
			current->size=current->size-smallest;
			new->size=smallest;
			new->occ=0;
			new->prev=current;
			new->next=current->next;
			current->next=new;
			if(new->next != NULL)
			{
				new->next->prev = new; 
			}
		}
	}
	else
	{
		current=sbrk(sizeof(struct Block)+size);
		current->occ =1;
		current->size=size+sizeof(struct Block);
		current->prev=temp;
		current->next = NULL;
		temp->next = current;
	}
	}
  return current+1;
}

/** @brief Mark the block free by unsetting the occ bit
 *
 *  In a real heap the freed block would be reused but
 *  this simple heap always allocats from the top of the
 *  heap by increasing heap space.
 *
 *  @param data Pointer to area being freed by user.
 *  @return Void.
 */
void my_free(void *data)
{
	struct Block *delete = (struct Block *)((char *)data-sizeof(struct Block));
	delete->occ=0;
	check_coalesce(delete);
	struct Block *temp;
	for(temp = head; temp->next != NULL; temp=temp->next)
	{
	}
	if(temp->occ==0)
		{
		if(temp==head)
		{
			sbrk(-(temp->size));
			head=NULL;
		}
		else {
			temp->prev->next=NULL;
			sbrk(-(temp->size));
		}
		}
}


void dump_heap()
{
  struct Block *cur;
  printf("brk: %p\n", sbrk(0));
  printf("head->");
  for(cur = head; cur != NULL; cur = cur->next) {
    printf("[%d:%d:%d]->", cur->occ, (char*)cur - (char*)head, cur->size);
    fflush(stdout);
    assert((char*)cur >= (char*)head && (char*)cur + cur->size <= (char*)sbrk(0)); // check that block is within bounds of the heap
    if(cur->next != NULL) {
      assert(cur->next->prev == cur); // if not last block, check that forward/backward links are consistent
      assert((char*)cur + cur->size == (char*)cur->next); // check that the block size is correctly set
    }
  }
  printf("NULL\n");
}

void check_coalesce(struct Block *pointer)
{
	if(pointer!=NULL)
	{
		if(pointer->prev != NULL){
		if(pointer->prev->occ==0)
		{
			pointer->prev->size=pointer->size+pointer->prev->size;
			if(pointer->next != NULL) {
			pointer->next->prev=pointer->prev;
			}
			pointer->prev->next=pointer->next;
			pointer=pointer->prev;
		}
		}
		if(pointer->next != NULL) {
		if(pointer->next->occ==0)
		{
			pointer->size=pointer->size+pointer->next->size;
			if(pointer->next->next != NULL) {
			pointer->next->next->prev=pointer;
			}
			pointer->next=pointer->next->next;
		}
		}
	}
}

#include<stdio.h>
#include<string.h>
#include "mem.h"
#include<stdlib.h>
#include<unistd.h>

#define MEMORY_SPACE 1024 // number of Default array size

struct memory_node {
  int process_id;
  int size;
  int start;
  struct memory_node *next;
};

struct memory_queue {
  int process_id;
  int size;
  struct memory_queue *next;
};





struct memory_node *node_current = NULL;

struct memory_queue *queue_head = NULL;
struct memory_queue *queue_current = NULL;

void initialize(){
  struct memory_node *node_head = (struct memory_node*) malloc(sizeof(struct memory_node));
  node_head->next = NULL;
  node_head->process_id = 0; // Empty Block
  node_head->size = MEMORY_SPACE;
  node_head->start = 0;
}

void allocateMem( int processID, int memSize ) {
  // Loop through the memory node to check for any empty blocks that have enough room for this one.

  // choose the first one that has memory by comparing the memory sizes.

  // if the empty block has enough memory,

  // subtract it's current empty block's size by the memSize

  // make the start of current empty block to it's original start address + memSize

  // now make the new process node's next to the new changed empty block.

  // now also
  struct memory_node *new_node = (struct memory_node*) malloc(sizeof(struct memory_node));
  node_head


}

void deleteMem( int processID ) {
  // loop through the memory node to check for a process with the processID.

  // set the process id to 0, to indicate that it's an empty block

}

void print() {

}




int main() {
  initialize();


}

#include<stdio.h>
#include<string.h>
#include "mem.h"
#include<stdlib.h>
#include<unistd.h>

#define MEMORY_SPACE 1000 // number of Default array size

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
struct memory_node *node_head;
struct memory_node *node_current = NULL;

struct memory_queue *queue_head = NULL;
struct memory_queue *queue_current = NULL;

void initialize(){
  node_head = (struct memory_node*) malloc(sizeof(struct memory_node));
  node_head->next = NULL;
  node_head->process_id = 0; // Empty Block
  node_head->size = MEMORY_SPACE;
  node_head->start = 0;
}

void allocateMem( int processID, int memSize ) {
  // Loop through the memory node to check for any empty blocks that have enough room for this one.
  printf("Add Process %d with memory %d\n", processID, memSize);
  node_current = node_head;
  while (node_current!= NULL){
    printf("size: %d\n", node_current->size);
    if (node_current->process_id == 0 && node_current->size >= memSize){
      printf("Enough memory\n");
      // since we need the context to previous node in order to connect from previous node to the new node,
      struct memory_node *new_empty_node = (struct memory_node*) malloc(sizeof(struct memory_node));
      // store the current empty node's size - newmem size to the new empty.
      new_empty_node->process_id = 0;
      new_empty_node->size = node_current->size - memSize;
      new_empty_node->start = node_current->start + memSize;
      new_empty_node->next = node_current->next;
      node_current->size = memSize;
      node_current->next = new_empty_node;
      node_current->process_id = processID;
      break;
    }else if (node_current->process_id == 0 && node_current->size < memSize){
      printf("Not enough memory\n");
    }else{

    }
    node_current = node_current->next;
  }

  // choose the first one that has memory by comparing the memory sizes.

  // if the empty block has enough memory,

  // subtract it's current empty block's size by the memSize

  // make the start of current empty block to it's original start address + memSize

  // now make the new process node's next to the new changed empty block.

  // now also loop through to check which one has the address of 0, and it will point to it.

  // The nodes that were pointing to the next one would be changed as well.

  // if the memory doesnt not have enough memory, store in to queue.

}

void deleteMem( int processID ) {
  // loop through the memory node to check for a process with the processID.

  // set the process id to 0, to indicate that it's an empty block

  // loop through the memory nodes to check if any two empty blocks are adjacent.

  // if they are, make it one empty memory block.

  // after removing, now loop through the queues to check if there any processes that can fit now. We can use this to call allocateMem
  node_current = node_head;
  while (node_current!= NULL){
    if (node_current->process_id == processID){
      node_current->process_id = 0;
    }
    node_current = node_current->next;
  }

  node_current = node_head;
  while (node_current!= NULL){
    if (node_current->process_id == 0){
      if (node_current->next != NULL){
        if (node_current->next->process_id == 0){
          printf("changed!\n");
          node_current->size = node_current->size + node_current->next->size;
          node_current->next = node_current->next->next;
        }
      }
    }
    node_current = node_current->next;
  }

}

void print() {
  printf("-------------------------------------------------------------------------------------------\n");
  printf("Allocation Queue: \n");
  node_current = node_head;
  while (node_current!= NULL){
    if (node_current->process_id > 0){
      printf("Process %d:           %dkB     Start Address: 0x%08X    End Address: 0x%08X\n", node_current->process_id, node_current->size, node_current->start * 1000, (node_current->start + node_current->size) * 1000 - 1);
    }else{
      printf("Empty Block:          %dkB     Start Address: 0x%08X    End Address: 0x%08X\n", node_current->size, node_current->start * 1000, (node_current->start + node_current->size) * 1000 - 1);
    }

    node_current = node_current->next;
  }

}




int main() {
  initialize();
  allocateMem(1, 400);
  allocateMem(2, 200);
  allocateMem(3, 500);
  print();
  deleteMem(1);
  print();
  allocateMem(4, 100);
  print();
  deleteMem(2);
  print();
  deleteMem(4);
  print();
  deleteMem(3);
  print();
  // deleteMem(1);



}

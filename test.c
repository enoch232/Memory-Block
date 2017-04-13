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
struct memory_queue *queue_head;
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
  // printf("Add Process %d with memory %d\n", processID, memSize);
  node_current = node_head;
  while (node_current!= NULL){
    if (node_current->process_id == 0 && node_current->size >= memSize){
      // printf("Enough memory\n");
      // since we need the context to previous node in order to connect from previous node to the new node,
      // struct memory_node *new_empty_node = (struct memory_node*) malloc(sizeof(struct memory_node));
      // // store the current empty node's size - newmem size to the new empty.
      // new_empty_node->process_id = 0;
      // new_empty_node->size = node_current->size - memSize;
      // new_empty_node->start = node_current->start + memSize;
      // new_empty_node->next = node_current->next;
      // node_current->size = memSize;
      // node_current->next = new_empty_node;
      // node_current->process_id = processID;
      if ((node_current->size - memSize) > 0){
        struct memory_node *new_empty_node = (struct memory_node*) malloc(sizeof(struct memory_node));
        new_empty_node->process_id = 0;
        new_empty_node->size = node_current->size - memSize;
        new_empty_node->start = node_current->start + memSize;
        new_empty_node->next = node_current->next;
        node_current->size = memSize;
        node_current->process_id = processID;
        node_current->next = new_empty_node;
      }else{
        node_current->size = memSize;
        node_current->process_id = processID;
      }

      break;


    }else if (node_current->process_id == 0 && node_current->size < memSize){

      if (queue_head == NULL){
        struct memory_queue *new_queue = (struct memory_queue*) malloc(sizeof(struct memory_queue));
        new_queue->process_id = processID;
        new_queue->size = memSize;
        new_queue->next = NULL;
        queue_head = new_queue;
      }else{
        queue_current = queue_head;
        while (queue_current!= NULL){
          if (queue_current->next== NULL){
            break;
          }
          queue_current = queue_current->next;

        }
        struct memory_queue *new_queue = (struct memory_queue*) malloc(sizeof(struct memory_queue));
        new_queue->process_id = processID;
        new_queue->size = memSize;
        new_queue->next = NULL;
        queue_current->next = new_queue;
      }

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

int allocateMemWithoutQueue( int processID, int memSize ) {
  // Loop through the memory node to check for any empty blocks that have enough room for this one.
  // printf("Add Process %d with memory %d\n", processID, memSize);
  node_current = node_head;
  int success = 0;
  while (node_current!= NULL){
    if (node_current->process_id == 0 && node_current->size >= memSize){
      if ((node_current->size - memSize) > 0){
        struct memory_node *new_empty_node = (struct memory_node*) malloc(sizeof(struct memory_node));
        new_empty_node->process_id = 0;
        new_empty_node->size = node_current->size - memSize;
        new_empty_node->start = node_current->start + memSize;
        new_empty_node->next = node_current->next;
        node_current->size = memSize;
        node_current->process_id = processID;
        node_current->next = new_empty_node;
      }else{
        node_current->size = memSize;
        node_current->process_id = processID;
      }
      success = 1;
      break;
    }else if (node_current->process_id == 0 && node_current->size < memSize){
    }else{
    }
    node_current = node_current->next;
  }
  if (success) {
    queue_current = queue_head;
    if (queue_current != NULL){
      queue_head = queue_current->next;
      allocateMemWithoutQueue(queue_current->process_id, queue_current->size);
    }

  }
  return success;

}

int mergeFreeMem() {
  node_current = node_head;
  int changed = 0;
  while (node_current!= NULL){
    if (node_current->process_id == 0){
      if (node_current->next != NULL){
        if (node_current->next->process_id == 0){
          // printf("changed!\n");
          node_current->size = node_current->size + node_current->next->size;
          node_current->next = node_current->next->next;
          changed = 1;

          // node_current = node_head;
        }
      }
    }
    node_current = node_current->next;
  }
  if (changed) {
    mergeFreeMem();
  }
  return changed;
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

  mergeFreeMem();
  if (queue_head != NULL){
    allocateMemWithoutQueue(queue_head->process_id, queue_head->size);

  }

}

void print() {
  printf("-------------------------------------------------------------------------------------------\n");
  printf("Allocation Queue: \n");
  node_current = node_head;
  while (node_current!= NULL){
    if (node_current->process_id > 0){
      printf("Process %d:            %dkB     Start Address: 0x%08X    End Address: 0x%08X\n", node_current->process_id, node_current->size, node_current->start * 1000, (node_current->start + node_current->size) * 1000 - 1);
    }else{
      printf("Empty Block:          %dkB     Start Address: 0x%08X    End Address: 0x%08X\n", node_current->size, node_current->start * 1000, (node_current->start + node_current->size) * 1000 - 1);
    }

    node_current = node_current->next;
  }
  printf("\nInput Queue: \n");
  queue_current = queue_head;
  while (queue_current!= NULL){
    printf("Process %d:           %dkB\n", queue_current->process_id, queue_current->size);
    queue_current = queue_current->next;
  }

}




int main() {
  initialize();
  allocateMem(1, 400);
  allocateMem(2, 400);
  allocateMem(3, 400);
  allocateMem(4, 400);

  deleteMem(1);
  deleteMem(2);
  // deleteMem(1);
  print();

  // allocateMem(1, 400);
  // allocateMem(2, 200);
  // allocateMem(3, 500);
  // print();
  // deleteMem(1);
  // print();
  // allocateMem(4, 100);
  // print();
  // deleteMem(2);
  // print();
  // deleteMem(4);
  // print();
  // deleteMem(3);
  // print();



}

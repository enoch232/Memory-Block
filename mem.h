#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#define MEMORY_SPACE 1000 // number of Default array size

struct memory_node {
  int process_id;
  int size;
  int start;
  struct memory_node *next;
}; //memory_node will be used to create a node for singly linkedlist to store memory.

struct memory_queue {
  int process_id; // process_id of 0 stands for empty memory.
  int size;
  struct memory_queue *next;
}; //memory_queue will be used to create a node for singly linkedlist to store queue.

struct memory_node *node_head; // init node_head to point to the first node for the memory
struct memory_node *node_current = NULL; // used for traversing through nodes for the memory
struct memory_queue *queue_head; // init queue_head to point to the first node (very first element to the queue)
struct memory_queue *queue_current = NULL; // used for traversing through nodes for the queue.

void initialize(){
  node_head = (struct memory_node*) malloc(sizeof(struct memory_node)); // create memory_node
  node_head->next = NULL; // no next.
  node_head->process_id = 0; // Empty Block
  node_head->size = MEMORY_SPACE; // uses default 1000.
  node_head->start = 0; // default of 0 address
}

void allocateMem( int processID, int memSize ) {
  // Loop through the memory node to check for any empty blocks that have enough room for this one.
  node_current = node_head;
  while (node_current!= NULL){
    if (node_current->process_id == 0 && node_current->size >= memSize){
      // When there is enough memory, and it's a empty memory,
      // since we need the context to previous node in order to connect from previous node to the new node,
      // store the current empty node's size - newmem size to the new empty.
      if ((node_current->size - memSize) > 0){ // if the new memory is less than the empty memory block,
        // create a new empty node, and connect the existing empty node to the new empty node as if it was a new block of memory.
        // This is to ensure that we have reference to the previous node, so that we can set the 'next'.
        struct memory_node *new_empty_node = (struct memory_node*) malloc(sizeof(struct memory_node));
        new_empty_node->process_id = 0; // create a new empty_node, process_of 0 is empty block
        new_empty_node->size = node_current->size - memSize; // update the empty block's new memory size.
        new_empty_node->start = node_current->start + memSize; // update the empty block's new start, it will be after the new node.
        new_empty_node->next = node_current->next; // point the empty node's to what old empty block was pointing to.
        node_current->size = memSize; //now node_current is the new memory block instead of empty block, so it will contain the memSize
        node_current->process_id = processID; //store processID
        node_current->next = new_empty_node; // now
      }else{ // if the new memory is equal to the empty memory block, don't create any space, but simply replace it.
        node_current->size = memSize;
        node_current->process_id = processID;
      }
      // break from the loop, to indicate that memory had found a suitable empty block to reside. No need to stay in the loop.
      break;


    }else if (node_current->process_id == 0 && node_current->size < memSize){ // if the empty block actually is smaller than the requested memory,
      // then we need to store the memory into the queue for later use.
      if (queue_head == NULL){ // if there are no elements in the queue,
        struct memory_queue *new_queue = (struct memory_queue*) malloc(sizeof(struct memory_queue)); //create a new queue.
        new_queue->process_id = processID; // store process info
        new_queue->size = memSize; // store process info
        new_queue->next = NULL; // next will point to NULL.
        queue_head = new_queue; // now THAT will be pointed by the head, since it's the first queue element, first to get out.
      }else{ // if there are more than one elements in the queue,
        queue_current = queue_head; // get the first element in the queue, and set the current_queue to that.
        // traverse until the last node, in order to add a new element to the end of linkedlist.
        while (queue_current!= NULL){
          if (queue_current->next== NULL){
            break;
          }
          queue_current = queue_current->next;

        }
        // create the new queue,
        struct memory_queue *new_queue = (struct memory_queue*) malloc(sizeof(struct memory_queue));
        new_queue->process_id = processID; // store process info
        new_queue->size = memSize; // store process info
        new_queue->next = NULL; // store process info
        queue_current->next = new_queue; // now set the next to the current pointer.
      }

    }else{
      // do no thing if it's not an empty block
    }
    // move to next.
    node_current = node_current->next;
  }
  // Algorithm:
  // choose the first one that has memory by comparing the memory sizes.

  // if the empty block has enough memory,

  // subtract it's current empty block's size by the memSize

  // make the start of current empty block to it's original start address + memSize

  // now make the new process node's next to the new changed empty block.

  // now also loop through to check which one has the address of 0, and it will point to it.

  // The nodes that were pointing to the next one would be changed as well.

  // if the memory doesnt not have enough memory, store in to queue.

}

// this allocateMem is used so that it doesn't create a queue for not being able to fit it in the memory.
int allocateMemWithoutQueue( int processID, int memSize ) {
  // Loop through the memory node to check for any empty blocks that have enough room for this one.
  node_current = node_head;
  int success = 0; //flag to check if memory has been allocated.
  while (node_current!= NULL){ // traverse through memory linkedlist
    if (node_current->process_id == 0 && node_current->size >= memSize){ // if the empty block has memory to fit the new allocated memory,
      if ((node_current->size - memSize) > 0){ // check if it fit's perfectly.
        struct memory_node *new_empty_node = (struct memory_node*) malloc(sizeof(struct memory_node)); // create new empty_node
        new_empty_node->process_id = 0;
        new_empty_node->size = node_current->size - memSize;
        new_empty_node->start = node_current->start + memSize;
        new_empty_node->next = node_current->next;
        node_current->size = memSize;
        node_current->process_id = processID;
        node_current->next = new_empty_node;
      }else{ // if it fit's perfectly in the empty memory, don't create any new empty memory block, and just replace it with memory block.
        node_current->size = memSize;
        node_current->process_id = processID;
      }
      success = 1;
      break;
    }else if (node_current->process_id == 0 && node_current->size < memSize){
      // do nothing
    }else{
      // do nothing if process id is not 0, meaning if the memory block is not an empty block...
    }
    node_current = node_current->next;
  }
  if (success) { // if there was an allocation of memory block,
    queue_current = queue_head;
    if (queue_current != NULL){ // check if the current queue pointed is not NULL, this is required so that ->next won't show error.
      queue_head = queue_current->next; // update the queue head, since queue head holds the first element that was added to the queue. Basically pops off the first one that came in.
      allocateMemWithoutQueue(queue_current->process_id, queue_current->size); // allocate memory with the new element now.
    }

  }
  return success; // return the flag

}

int mergeFreeMem() { // this method will find any adjacent empty memory blocks and combine them.
  node_current = node_head; // point to the first element
  int changed = 0; // indicator to check if nothing had changed.
  while (node_current!= NULL){ // traverse
    if (node_current->process_id == 0){ // if it's an empty memory block
      if (node_current->next != NULL){ // make sure that there is a such memory block after
        if (node_current->next->process_id == 0){ // check also if the next one is an empty memory block,
          node_current->size = node_current->size + node_current->next->size; // add empty memory blocks.
          node_current->next = node_current->next->next; //remove the context for other empty memory node.
          changed = 1; // indicate that there has been an empty memory join.
        }
      }
    }
    node_current = node_current->next;
  }
  if (changed) {
    mergeFreeMem(); // recursive call, if any empty memory has combined. This makes it possible that there could be more.
  }
  return changed; // return the changed value.
}

void deleteMem( int processID ) {
  node_current = node_head;
  // loop through the memory node to check for a process with the processID.
  while (node_current!= NULL){
    if (node_current->process_id == processID){
      // set the process id to 0, to indicate that it's an empty block
      node_current->process_id = 0;
    }
    node_current = node_current->next;
  }
  // loop through the memory nodes to check if any two empty blocks are adjacent. if they are, make it one empty memory block.
  mergeFreeMem();
  // after removing, now loop through the queues to check if there any processes that can fit now.
  if (queue_head != NULL){
    allocateMemWithoutQueue(queue_head->process_id, queue_head->size);

  }

}

void print() {
  printf("-------------------------------------------------------------------------------------------\n");
  printf("Allocation Queue: \n");
  node_current = node_head; // point to the very first node, in order to traverse.
  while (node_current!= NULL){ // traverse through linked list for the memory
    if (node_current->process_id > 0){
      printf("Process %d:            %dkB     Start Address: 0x%08X    End Address: 0x%08X\n", node_current->process_id, node_current->size, node_current->start * 1000, (node_current->start + node_current->size) * 1000 - 1);
    }else{
      printf("Empty Block:          %dkB     Start Address: 0x%08X    End Address: 0x%08X\n", node_current->size, node_current->start * 1000, (node_current->start + node_current->size) * 1000 - 1);
    }

    node_current = node_current->next;
  }
  printf("\nInput Queue: \n");
  queue_current = queue_head; // point to the very first queue node, in order to traverse.
  while (queue_current!= NULL){
    printf("Process %d:           %dkB\n", queue_current->process_id, queue_current->size);
    queue_current = queue_current->next;
  }

}

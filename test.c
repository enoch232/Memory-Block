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

struct memory_node *node_head = NULL;
struct memory_node *node_current = NULL;

struct memory_queue *queue_head = NULL;
struct memory_queue *queue_current = NULL;

void allocateMem( int processID, int memSize ) {

}

void deleteMem( int processID ) {

}

void print() {

}




int main() {

}

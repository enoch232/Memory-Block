
#include "mem.h"



int main() {
  // this initializes the program to set up the initial memory space.
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
}

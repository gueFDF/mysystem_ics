#ifndef DRAM_GUARD
#define DRAM_GUARD
#include<stdlib.h>
#include<stdint.h>
#define MM_LEN 1000


uint8_t mm[MM_LEN];//假内存  piscycal memory

void print_stack();
void print_register();
#endif
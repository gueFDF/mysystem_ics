#ifndef INSTRUCTION_GUARD
#define INSTRUCTION_GUARD
#include "cpu/register.h"
#include <stdlib.h>
#include <stdint.h>
#define NUM_OP 30
typedef enum OP
{
    mov,
    add_reg_reg,
    mov_reg_reg,
    push_reg,
    mov_reg_mem,
    mov_mem_reg,
    pop_reg,
    ret,
    call,
} op_t;

typedef enum NUM_INSTRUCTION //操作种类
{
    EMPTY,
    IMM,
    REG,
    MM_IMM,
    MM_REG,
    MM_IMM_REG,
    MM_REG1_REG2,
    MM_IMM_REG1_REG2,
    MM_REG2_S,
    MM_IMM_REG2_S,
    MM_REG1_REG2_S,
    MM_IMM_REG1_PEG2_S,
} od_type_t;
typedef struct OD // 一个操作数
{
    od_type_t type;
    uint64_t imm;
    uint64_t scal;
    uint64_t *reg1;
    uint64_t *reg2;
} od_t;
typedef struct INSTRUCT_STRUCT //一条指令
{
    op_t op; // mov,push.....
    od_t src;
    od_t dst;
    char code[100];
} inst_t;

typedef void (*handler_t)(uint64_t, uint64_t);

//指令周期
/*
while (1)
{
  从PC指示的存储器位置取出指令;
  执行指令;
  更新PC;
}
*/
void instruction_cyale();

void add_reg_reg_handler(uint64_t src, uint64_t dst);
void mov_reg_reg_handler(uint64_t src, uint64_t dst);
void push_handler(uint64_t src, uint64_t dst);
void call_handler(uint64_t src, uint64_t dst);
void pop_handler(uint64_t src, uint64_t dst);
void mov_reg_mem_handler(uint64_t src, uint64_t dst);
void mov_mem_reg_handler(uint64_t src, uint64_t dst);
void ret_handler(uint64_t src, uint64_t dst);
handler_t handler_table[NUM_OP];
void init_handler_table(); //初始化handler_table

//读写内存
uint64_t read64bit_dram(uint64_t paddr);
void write64bits_dram(uint64_t paddr, uint64_t data);

#endif
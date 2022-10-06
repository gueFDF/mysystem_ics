#include <stdlib.h>
#include <stdint.h>

typedef enum OP
{
    MOV,
    PUSH,
    CALL,
} op_t;

typedef enum OD_TYPE //操作种类
{
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
    MM_IMM_REG1_PEG2_S
} od_type_t;
typedef struct OD  // 一个操作
{
    od_type_t type;
    uint64_t imm;
    uint64_t scal;
    uint64_t* reg1;
    uint64_t* reg2;
    char code[100];
} od_t;
typedef struct INSTRUCT_STRUCT //一条指令
{
    op_t op; // mov,push.....
    od_t src;
    od_t dst;
} inst_t;


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
#include <stdlib.h>
#include <stdint.h>
#define MM_LEN 1000
#define INST_LEN 100

uint8_t mm[MM_LEN];//假内存


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
    MM_REG1_REG2_s,
    MM_IMM_REG1_PEG2_S
} od_type_t;
typedef struct OD  // 一个操作
{
    od_type_t type;
    uint64_t imm;
    uint64_t scal;
    uint64_t reg1;
    uint64_t reg2;
} od_t;
typedef struct INSTRUCT_STRUCT //一条指令
{
    op_t op; // mov,push.....
    od_t src;
    od_t dst;
} inst_t;



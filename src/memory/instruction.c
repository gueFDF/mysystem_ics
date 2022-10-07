#include "memory/instruction.h"
#include "cpu/mmu.h"
#include "cpu/register.h"
static uint64_t decode_od(od_t od)
{
    //立即数
    if (od.type == IMM)
    {
        return od.imm;
    }
    else if (od.type == REG) //寄存器
    {
        return (uint64_t)od.reg1;
    }
    else //下面就是访问内存
    {
        uint64_t vaddr = 0;
        if (od.type == MM_IMM)
        {
            vaddr = *((uint64_t *)(&od.imm));
        }
        else if (od.type == MM_REG)
        {
            vaddr = *(od.reg1);
        }
        else if (od.type == MM_IMM_REG)
        {
            vaddr = od.imm + *(od.reg1);
        }
        else if (od.type == MM_REG1_REG2)
        {
            vaddr = *(od.reg1) + *(od.reg2);
        }
        else if (od.type == MM_IMM_REG1_REG2)
        {
            vaddr = od.imm + *(od.reg1) + *(od.reg2);
        }
        else if (od.type == MM_REG2_S)
        {
            vaddr = (*(od.reg2)) * od.scal;
        }
        else if (od.type == MM_IMM_REG2_S)
        {
            vaddr = od.imm + (*(od.reg2)) * od.scal;
        }
        else if (od.type == MM_REG1_REG2_S)
        {
            vaddr = *(od.reg1) + (*(od.reg2)) * od.scal;
        }
        else if (od.type == MM_IMM_REG1_PEG2_S)
        {
            vaddr = od.imm + *(od.reg1) + (*(od.reg2)) * od.scal;
        }
        else if (od.type == EMPTY)
        {
            vaddr = 0;
        }
        return vaddr;
    }
}

void instruction_cyale()
{
    inst_t *instr = (inst_t *)reg.rip; //  取指
    printf("op:%d\n", instr->op);
    uint64_t src = decode_od(instr->src); //译码
    uint64_t dst = decode_od(instr->dst);
    handler_table[instr->op](src, dst); //执行

    printf("     %s\n", instr->code); //打印汇编操作，方便查看
}

void init_handler_table()
{
    handler_table[add_reg_reg] = add_reg_reg_handler;
    handler_table[mov_reg_reg] = mov_reg_reg_handler;
    handler_table[call] = call_handler;
    handler_table[push_reg] = push_handler;
    handler_table[mov_reg_mem] = mov_reg_mem_handler;
    handler_table[mov_mem_reg] = mov_mem_reg_handler;
    handler_table[pop_reg] = pop_handler;
    handler_table[ret] = ret_handler;
}
void add_reg_reg_handler(uint64_t src, uint64_t dst)
{
    *(uint64_t *)dst = *(uint64_t *)src + *(uint64_t *)dst;
    reg.rip += sizeof(inst_t); //让pc指向下一条指令
}

void mov_reg_reg_handler(uint64_t src, uint64_t dst)
{
    *(uint64_t *)dst = *(uint64_t *)src;
    reg.rip += sizeof(inst_t); //让pc指向下一条指令
}
void mov_reg_mem_handler(uint64_t src, uint64_t dst)
{
    write64bits_dram(va2pa(dst), *(uint64_t *)src);
    reg.rip += sizeof(inst_t); //让pc指向下一条指令
}
void mov_mem_reg_handler(uint64_t src, uint64_t dst)
{
    *(uint64_t *)dst = read64bit_dram(va2pa(src));
    reg.rip += sizeof(inst_t); //让pc指向下一条指令
}
void call_handler(uint64_t src, uint64_t dst)
{
    // src:imm address of called function
    //向下压栈
    reg.rsp -= 8;
    //将返回地址存在栈顶,也就是call指令的下一条指令
    write64bits_dram(va2pa(reg.rsp), reg.rip + sizeof(inst_t));
    //更新rip，使它执行跳转地址
    reg.rip = src;
}
void push_handler(uint64_t src, uint64_t dst)
{
    // src:reg
    // dst:empty
    //会先压栈
    reg.rsp -= 8;
    //将src存放在栈顶
    write64bits_dram(va2pa(reg.rsp), *(uint64_t *)src);
    reg.rip += sizeof(inst_t); //让pc指向下一条指令
}

void pop_handler(uint64_t src, uint64_t dst)
{
    // src:reg
    // dst:empty
    //将返回值存放在src中
    *(uint64_t *)src = read64bit_dram(va2pa(reg.rsp));
    //弹栈
    reg.rsp += 8;
    reg.rip += sizeof(inst_t); //让pc指向下一条指令
}
void ret_handler(uint64_t src, uint64_t dst)
{
    // src:reg
    // dst:empty
    reg.rip = read64bit_dram(va2pa(reg.rsp));
    reg.rsp += 8;
}
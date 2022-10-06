#include<stdio.h>
#include"cpu/register.h"
#include"memory/instruction.h"
int main()
{
    return 0;
}
uint64_t deode_od(od_t od)
{
    if(od.type==IMM)
    {
        return od.imm;
    }
    else if(od.type==REG)
    {
        return od.reg1;
    }
    return 0;
}
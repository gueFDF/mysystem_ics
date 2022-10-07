#include<stdio.h>
#include"cpu/register.h"
#include"memory/instruction.h"
#include"memory/dram.h"
#include"cpu/mmu.h"
#include"disk/elf.h"
int main()
{   
    reg.rax=0x12340000;
    reg.rbx=0x0;
    reg.rcx=0x8000660;
    reg.rdx=0xabcd;
    reg.rsi=0x7ffffffee2f8;
    reg.rdi=0x1;
    reg.rbp=0x7ffffffee210;
    reg.rsp=0x7ffffffee1f0;

    reg.rip=(uint64_t)&program[11];  //pc指针


    write64bits_dram(va2pa(0x7ffffffee210),0x08000660);//rbp指向的内存
    write64bits_dram(va2pa(0x7ffffffee208),0x0);
    write64bits_dram(va2pa(0x7ffffffee200),0xabcd);
    write64bits_dram(va2pa(0x7ffffffee1f8),0x12340000);
    write64bits_dram(va2pa(0x7ffffffee1f0),0x08000660);//rsp指向栈顶


    printf("%16lx\n",*((uint64_t*)&(mm[va2pa(0x7ffffffee1f8)])));
    printf("%16lx\n",read64bit_dram(va2pa(0x7ffffffee1f8)));
    //run inst
    for(int i=0;i<0;i++)
    {
        instruction_cyale();
    }
    
    print_stack();

    return 0;
}

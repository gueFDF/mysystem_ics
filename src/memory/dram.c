#include "memory/dram.h"
#include "cpu/register.h"
#include "cpu/mmu.h"
#define SRAN_CACHE_SETTING 0 // fliging
uint64_t read64bit_dram(uint64_t paddr)
{
    if (SRAN_CACHE_SETTING == 1)
    {
        return 0;
    }
    uint64_t data = 0x0;
    for (int i = 0; i < 8; i++)
    {
        data += ((uint64_t)mm[paddr + i]) << (i * 8);
    }
    return data;
}
void write64bits_dram(uint64_t paddr, uint64_t data)
{
    if (SRAN_CACHE_SETTING == 1)
    {
        return;
    }
    for (int i = 0; i < 8; i++)
    {
        mm[paddr + i] = (data >> (i * 8)) & 0xff;
    }
}
//打印寄存器
void print_register()
{
    printf("rax = %16lx\trbx = %16lx\trcx = %16lx\trdx = %16lx\n",
           reg.rax, reg.rbx, reg.rcx, reg.rdx);
    printf("rsi = %16lx\trdi = %16lx\trbp = %16lx\trsp = %16lx\n",
           reg.rsi, reg.rdi, reg.rbp, reg.rsp);
    printf("rip = %16lx\n", reg.rip);
}

//打印栈
void print_stack()
{
    int n = 10;

    uint64_t *high = (uint64_t *)&mm[va2pa(reg.rsp)];
    high = &high[n];
    uint64_t rsp_start = reg.rsp + n * 8;

    for (int i = 0; i < 2 * n; ++i)
    {
        uint64_t *ptr = (uint64_t *)(high - i);
        printf("0x%016lx : %16lx", rsp_start, (uint64_t)*ptr);

        if (i == n)
        {
            printf(" <== rsp");
        }
        if(i==6)
        {
            printf(" <== rbp");
        }

        rsp_start = rsp_start - 8;

        printf("\n");
    }
}
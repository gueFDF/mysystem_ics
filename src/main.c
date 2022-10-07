#include <stdio.h>
#include "cpu/register.h"
#include "memory/instruction.h"
#include "memory/dram.h"
#include "cpu/mmu.h"
#include "disk/elf.h"
int main()
{
    reg.rax = 0x12340000;
    reg.rbx = 0x0;
    reg.rcx = 0x8000660;
    reg.rdx = 0xabcd;
    reg.rsi = 0x7ffffffee2f8;
    reg.rdi = 0x1;
    reg.rbp = 0x7ffffffee210;
    reg.rsp = 0x7ffffffee1f0;

    reg.rip = (uint64_t)&program[11]; // pc指针

    init_handler_table();
    write64bits_dram(va2pa(0x7ffffffee210), 0x08000660); // rbp指向的内存
    write64bits_dram(va2pa(0x7ffffffee208), 0x0);
    write64bits_dram(va2pa(0x7ffffffee200), 0xabcd);
    write64bits_dram(va2pa(0x7ffffffee1f8), 0x12340000);
    write64bits_dram(va2pa(0x7ffffffee1f0), 0x08000660); // rsp指向栈顶

    // run inst
    print_stack();
    print_register();
    for (int i = 0; i < 15; i++)
    {
        instruction_cyale();
        print_stack();
        print_register();
    }
    //以下代码使用来检测是否与电脑最终执行出来的及结果一致
    int match = 1;

    match = match && (reg.rax == 0x1234abcd);
    match = match && (reg.rbx == 0x0);
    match = match && (reg.rcx == 0x8000660);
    match = match && (reg.rdx == 0x12340000);
    match = match && (reg.rsi == 0xabcd);
    match = match && (reg.rdi == 0x12340000);
    match = match && (reg.rbp == 0x7ffffffee210);
    match = match && (reg.rsp == 0x7ffffffee1f0);

    if (match == 1)
    {
        printf("register match\n");
    }
    else
    {
        printf("register not match\n");
    }
    match = 1;
    match = match && (read64bit_dram(va2pa(0x7ffffffee210)) == 0x08000660); // rbp
    match = match && (read64bit_dram(va2pa(0x7ffffffee208)) == 0x1234abcd);
    match = match && (read64bit_dram(va2pa(0x7ffffffee200)) == 0xabcd);
    match = match && (read64bit_dram(va2pa(0x7ffffffee1f8)) == 0x12340000);
    match = match && (read64bit_dram(va2pa(0x7ffffffee1f0)) == 0x08000660);

    if (match == 1)
    {
        printf("memory match\n");
    }
    else
    {
        printf("memory not match\n");
    }

    return 0;
}

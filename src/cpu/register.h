#ifndef REG_GUARD
#define REG_GUARD
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct CPU_STRUCT //模拟CPU(统用寄存器)
{
    union //返回值
    {
        struct
        {
            uint8_t al; //搞地址
            uint8_t ah; //低地址
        };
        uint16_t ax;
        uint32_t eax;
        uint64_t rax;
    };
    union //被调用者保存
    {
        struct
        {
            uint8_t bl; //搞地址
            uint8_t bh; //低地址
        };
        uint16_t bx;
        uint32_t ebx;
        uint64_t rbx;
    };
    union //第四个参数
    {
        struct
        {
            uint8_t cl; //搞地址
            uint8_t ch; //低地址
        };
        uint16_t cx;
        uint32_t ecx;
        uint64_t rcx;
    };
    union //第三个参数
    {
        struct
        {
            uint8_t dl; //搞地址
            uint8_t dh; //低地址
        };
        uint16_t dx;
        uint32_t edx;
        uint64_t rdx;
    };
    union //第二个参数
    {
        struct
        {
            uint8_t sil; //搞地址
            uint8_t sih; //低地址
        };
        uint16_t si;
        uint32_t esi;
        uint64_t rsi;
    };
    union //第一个参数
    {
        struct
        {
            uint8_t dil; //搞地址
            uint8_t dih; //低地址
        };
        uint16_t di;
        uint32_t edi;
        uint64_t rdi;
    };
    union //被调用者保存
    {
        struct
        {
            uint8_t bpl; //搞地址
            uint8_t bph; //低地址
        };
        uint16_t bp;
        uint32_t ebp;
        uint64_t rbp;
    };
    union //栈指针
    {
        struct
        {
            uint8_t spl; //搞地址
            uint8_t sph; //低地址
        };
        uint16_t sp;
        uint32_t esp;
        uint64_t rsp;
    };

    uint64_t rip; // pc寄存器

} CPU;

CPU reg; 


#endif
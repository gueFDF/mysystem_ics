#include<headers/cpu.h>
#include<headers/memory.h>
#include<headers/common.h>
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
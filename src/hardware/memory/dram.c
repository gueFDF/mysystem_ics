#include<headers/cpu.h>
#include<headers/memory.h>
#include<headers/common.h>
uint64_t read64bits_dram(uint64_t paddr,core_t *cr)
{
    if (DEBUG_ENABLE_SRAM_CACHE == 1)
    {
        return 0;
    }
    uint64_t data = 0x0;
    for (int i = 0; i < 8; i++)
    {
        data += ((uint64_t)pm[paddr + i]) << (i * 8);
    }
    return data;
}
void write64bits_dram(uint64_t paddr, uint64_t data,core_t *cr)
{
    if (DEBUG_ENABLE_SRAM_CACHE == 1)
    {
        return;
    }
    for (int i = 0; i < 8; i++)
    {
        pm[paddr + i] = (data >> (i * 8)) & 0xff;
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <headers/common.h>

// covert string to int64_t
uint64_t string2uint(const char *str) //不指定范围解析
{
    return string2uint_range(str, 0, -1);
}

uint64_t string2uint_range(const char *str, int start, int end) //指定范围解析
{
    //规定若end为-1就指的是该字符串结尾
    //进行判断end
    end = (end == -1) ? strlen(str) - 1 : end;
    uint64_t uv = 0;                   //存放返回值
    int stat = 0;                      //状态码，解析该串采用状态机的思想
    int sign_bit = 0;                  //标志符号位   0位正数，1位负数
    for (int i = start; i <= end; i++) //循环解析
    {
        char c = str[i];
        if (stat == 0)
        {
            if (c == '0')
            {
                stat = 1;
                uv = 0;
                continue;
            }
            else if (c >= '0' && c <= '9')
            {
                stat = 2;
                uv = c - '0';
                continue;
            }
            else if (c == '-')
            {
                stat = 3;
                sign_bit = 1;
                uv = 0;
                continue;
            }
            else if (c == ' ') //空格就不该变状态
            {
                continue;
            }
            else
            {
                goto fail;
            }
        }
        else if (stat == 1)
        {
            if (c == 'x') //十六进制
            {
                stat = 4;
                continue;
            }
            else if (c >= '0' && c <= '9')
            {
                stat = 2;
                uv = uv * 10 + c - '0';
                continue;
            }
            else if (c == ' ') //解析完成 进入结束状态
            {
                stat = 6;
                continue;
            }
            else
            {
                goto fail;
            }
        }
        else if (stat == 2)
        {
            if (c >= '0' && c <= '9')
            {
                stat = 2;
                uint64_t pv = uv;
                uv = uv * 10 + c - '0';
                if (pv > uv) //判断是否溢出
                {
                    printf("mybe overflow\n");
                    goto fail;
                }
                continue;
            }
            else if (c == ' ') //解析完成 进入结束状态
            {
                stat = 6;
                continue;
            }
            else
            {
                goto fail;
            }
        }
        else if (stat == 3)
        {
            if (c == '0')
            {
                stat = 1;
                continue;
            }
            else if (c >= '1' && c <= '9')
            {
                stat = 2;
                uv = uv * 10 + c - '0';
                continue;
            }
            else if (c == ' ') //解析完成 进入结束状态
            {
                stat = 6;
                continue;
            }
            else
            {
                goto fail;
            }
        }
        else if (stat == 4)
        {
            if ((c >= '0' && c <= '9'))
            {
                stat = 5;
                uv = uv * 16 + c - '0';
                continue;
            }
            else if (c >= 'a' && c <= 'f')
            {
                stat = 5;
                uv = uv * 16 + c - 'a' + 10;
                continue;
            }
            else if (c == ' ') //解析完成 进入结束状态
            {
                stat = 6;
                continue;
            }
            else
            {
                goto fail;
            }
        }
        else if (stat == 5)
        {
            if ((c >= '0' && c <= '9'))
            {
                stat = 5;
                uint64_t pv = uv;
                uv = uv * 16 + c - '0';
                if (pv > uv)
                {
                    printf("%c\n", c);
                    printf("pv:%lu uv:%lu\n", pv, uv);
                    printf("mybe overflow\n");
                    goto fail;
                }
                continue;
            }
            else if (c >= 'a' && c <= 'f')
            {
                stat = 5;
                uint64_t pv = uv;
                uv = uv * 16 + c - 'a' + 10;
                if (pv > uv)
                {
                    printf("pv:%lu uv:%lu\n", pv, uv);
                    printf("mybe overflow\n");
                    goto fail;
                }
                continue;
            }
            else if (c == ' ') //解析完成 进入结束状态
            {
                stat = 6;
                continue;
            }
            else
            {
                goto fail;
            }
        }
        else if (stat == 6) //进入结束状态
        {
            if (c == ' ') //解析完成 进入结束状态
            {
                stat = 6;
                continue;
            }
            else
            {
                goto fail;
            }
        }
    }
    if (sign_bit == 0) //判断符号标志，为0代表是正数
    {
        return uv;
    }
    else //负数
    {
        if ((uv & 8000000000000000) != 0) //说明最高位被占用，溢出
        {
            printf("最高位被占用\n");
            goto fail;
        }

        int64_t sv = -1 * (int64_t)uv;
        return *((uint64_t *)&sv);
    }

fail:
    printf("error str <%s>\n", str);
    exit(0);
}

// convert uint32_t to its float
uint32_t uint2float(uint32_t u)
{
    if (u == 0x00000000)
    {
        return 0x00000000;
    }
    // must be NORMALIZED
    // counting the position of highest 1: u[n]
    int n = 31;
    while (0 <= n && (((u >> n) & 0x1) == 0x0))
    {
        n = n - 1;
    }
    uint32_t e, f;
    //    seee eeee efff ffff ffff ffff ffff ffff
    // <= 0000 0000 1111 1111 1111 1111 1111 1111
    if (u <= 0x00ffffff)
    {
        // no need rounding
        uint32_t mask = 0xffffffff >> (32 - n);
        f = (u & mask) << (23 - n);
        e = n + 127;
        return (e << 23) | f;
    }
    // >= 0000 0001 0000 0000 0000 0000 0000 0000
    else
    {
        // need rounding
        // expand to 64 bit for situations like 0xffffffff
        uint64_t a = 0;
        a += u;
        // compute g, r, s
        uint32_t g = (a >> (n - 23)) & 0x1; // Guard bit, the lowest bit of the result
        uint32_t r = (a >> (n - 24)) & 0x1; // Round bit, the highest bit to be removed
        uint32_t s = 0x0;                   // Sticky bit, the OR of remaining bits in the removed part (low)
        for (int j = 0; j < n - 24; ++j)
        {
            s = s | ((u >> j) & 0x1);
        }
        // compute carry
        a = a >> (n - 23);
        // 0    1    ?    ... ?
        // [24] [23] [22] ... [0]
        /* Rounding Rules
            +-------+-------+-------+-------+
            |   G   |   R   |   S   |       |
            +-------+-------+-------+-------+
            |   0   |   0   |   0   |   +0  | round down
            |   0   |   0   |   1   |   +0  | round down
            |   0   |   1   |   0   |   +0  | round down
            |   0   |   1   |   1   |   +1  | round up
            |   1   |   0   |   0   |   +0  | round down
            |   1   |   0   |   1   |   +0  | round down
            |   1   |   1   |   0   |   +1  | round up
            |   1   |   1   |   1   |   +1  | round up
            +-------+-------+-------+-------+
        carry = R & (G | S) by K-Map
        */
        if ((r & (g | s)) == 0x1)
        {
            a = a + 1;
        }
        // check carry
        if ((a >> 23) == 0x1)
        {
            // 0    1    ?    ... ?
            // [24] [23] [22] ... [0]
            f = a & 0x007fffff;
            e = n + 127;
            return (e << 23) | f;
        }
        else if ((a >> 23) == 0x2)
        {
            // 1    0    0    ... 0
            // [24] [23] [22] ... [0]
            e = n + 1 + 127;
            return (e << 23);
        }
    }
    // inf as default error
    return 0x7f800000;
}
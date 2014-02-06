/* ===-- lshrdi3.c - Implement __lshrdi3 -----------------------------------===
 *
 *                     The LLVM Compiler Infrastructure
 *
 * This file is dual licensed under the MIT and the University of Illinois Open
 * Source Licenses. See LICENSE.TXT for details.
 *
 * ===----------------------------------------------------------------------===
 *
 * This file implements __lshrdi3 for the compiler_rt library.
 *
 * ===----------------------------------------------------------------------===
 */

typedef      int si_int;
typedef unsigned su_int;

typedef          long long di_int;
typedef unsigned long long du_int;

struct s_{
#if 1
        su_int low;
        su_int high;
#else
        su_int high;
        su_int low;
#endif 
};

typedef union
{
    du_int all;
    struct s_ s;
} udwords;

#ifndef CHAR_BIT
#define CHAR_BIT 8 
#endif

//lshrdi3
di_int __aeabi_llsr(di_int a, si_int b) {
    const int bits_in_word = (int)(sizeof(si_int) * CHAR_BIT);
    udwords input;
    udwords result;
    input.all = a;
    if (b & bits_in_word)  /* bits_in_word <= b < bits_in_dword */
    {
        result.s.high = 0;
        result.s.low = input.s.high >> (b - bits_in_word);
    }
    else  /* 0 <= b < bits_in_word */
    {
        if (b == 0)
            return a;
        result.s.high  = input.s.high >> b;
        result.s.low = (input.s.high << (bits_in_word - b)) | (input.s.low >> b);
    }
    return result.all;
}

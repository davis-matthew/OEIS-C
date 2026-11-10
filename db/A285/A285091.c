/*
 * A285091  Positions of 0 in A284848; complement of A285092.
 * https://oeis.org/A285091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A284848, where A284848 is the
 *            Thue-Morse word with every 1100 replaced by 1 (computed by
 *            scanning)
 * Range:     u8 n <= 114, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9055 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static inline int A285091_tm(idx_t pos)
{
    int p = 0;
    while (pos) { p ^= (int)(pos & 1); pos >>= 1; }
    return p;
}

seq_t a(idx_t n)
{
    static idx_t *zpos = NULL;
    static idx_t cnt = 0, cap = 0;
    static idx_t next_input = 0;
    static idx_t outpos = 0;

    while (cnt < n)
    {
        int b0 = A285091_tm(next_input);
        int b1 = A285091_tm(next_input + 1);
        int b2 = A285091_tm(next_input + 2);
        int b3 = A285091_tm(next_input + 3);
        outpos++;
        if (b0 == 1 && b1 == 1 && b2 == 0 && b3 == 0)
        {
            next_input += 4;
        }
        else
        {
            if (b0 == 0)
            {
                if (cnt == cap)
                {
                    idx_t nc = cap ? cap * 2 : 64;
                    idx_t *t = (idx_t *)realloc(zpos, (size_t)nc * sizeof(idx_t));
                    if (!t) return (seq_t)0;
                    zpos = t;
                    cap = nc;
                }
                zpos[cnt++] = outpos;
            }
            next_input += 1;
        }
    }
    return (seq_t)zpos[n - 1];
}

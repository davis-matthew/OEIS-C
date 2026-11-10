/*
 * A287410  Positions of 2 in A287407.
 * https://oeis.org/A287410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position (1-based) of the n-th 2 in the fixed point of
 *            0->012, 1->210, 2->201 (A287407); computed by scanning the word,
 *            with symbol i found from the base-3 digits of i
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9978 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static int A287410_sym(uint64_t i)
{
    static const int M[3][3] = { {0,1,2}, {2,1,0}, {2,0,1} };
    int d[48];
    int k = 0;
    int s = 0;
    while (i > 0) {
        d[k++] = (int)(i % 3);
        i /= 3;
    }
    while (k > 0) {
        s = M[s][d[--k]];
    }
    return s;
}

seq_t a(idx_t n)
{
    static uint64_t *pos = NULL;
    static size_t cnt = 0, cap = 0;
    static uint64_t next = 0;

    if (n < 1)
        return (seq_t)0;

    while (cnt < (size_t)n) {
        if (A287410_sym(next) == 2) {
            if (cnt == cap) {
                size_t nc = cap ? cap * 2 : 1024;
                uint64_t *t = (uint64_t *)realloc(pos, nc * sizeof(uint64_t));
                if (!t)
                    return (seq_t)0;
                pos = t;
                cap = nc;
            }
            pos[cnt++] = next + 1;
        }
        next++;
    }
    return (seq_t)pos[(size_t)n - 1];
}

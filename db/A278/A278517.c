/*
 * A278517  a(n) = smallest k for which A260731(k) = n.
 * https://oeis.org/A278517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k with A260731(k) = n, where A260731(0)=0 and
 *            A260731(k) = 1 + A260731(k - A002828(k)); computed by scanning k
 *            upward with a sieve
 * Range:     u8 n <= 91, u16 n <= 11363, u32 n <= 11363, u64 n <= 11363,
 *            u128 n <= 11363, big any n
 * Validated: standard, pass, 90 terms through n = 9592 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned int *A278517_g = NULL;
static unsigned int *A278517_st = NULL;
static unsigned int *A278517_first = NULL;
static unsigned long A278517_cap = 0;
static unsigned long A278517_K = 0; /* last processed k */

static void A278517_grow(void)
{
    unsigned long nc = A278517_cap ? A278517_cap * 2 : 1024;
    A278517_g = (unsigned int *)realloc(A278517_g, nc * sizeof(unsigned int));
    A278517_st = (unsigned int *)realloc(A278517_st, nc * sizeof(unsigned int));
    A278517_first = (unsigned int *)realloc(A278517_first, nc * sizeof(unsigned int));
    for (unsigned long i = A278517_cap; i < nc; ++i)
        A278517_first[i] = 0xFFFFFFFFu;
    if (A278517_cap == 0) {
        A278517_g[0] = 0;
        A278517_st[0] = 0;
        A278517_first[0] = 0;
    }
    A278517_cap = nc;
}

seq_t a(idx_t n)
{
    if (A278517_cap == 0)
        A278517_grow();
    while (n >= (idx_t)A278517_cap || A278517_first[(unsigned long)n] == 0xFFFFFFFFu) {
        unsigned long k = A278517_K + 1;
        if (k >= A278517_cap)
            A278517_grow();
        unsigned int best = (unsigned int)k;
        for (unsigned long j = 1; j * j <= k; ++j) {
            unsigned int c = A278517_g[k - j * j] + 1;
            if (c < best)
                best = c;
        }
        A278517_g[k] = best;
        A278517_st[k] = A278517_st[k - best] + 1;
        if (A278517_first[A278517_st[k]] == 0xFFFFFFFFu)
            A278517_first[A278517_st[k]] = (unsigned int)k;
        A278517_K = k;
    }
    return (seq_t)A278517_first[(unsigned long)n];
}

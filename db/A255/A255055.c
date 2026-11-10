/*
 * A255055  Greatest inverse of A255072; a(n) = largest k such that A255072(k)
 *          = n.
 * https://oeis.org/A255055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n) = largest k with A255072(k) = n, found by sieve:
 *            steps(k) = steps(k - runs(k)) + 1, runs(k) = popcount(k ^
 *            (k>>1)); a(n) = last k with steps = n
 * Range:     u8 n <= 65, u16 n <= 8590, u32 n <= 16143, u64 n <= 16143,
 *            u128 n <= 16143, big any n
 * Validated: standard, pass, 83 terms through n = 9447 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint32_t *A255055_st = NULL;
static size_t A255055_len = 0, A255055_cap = 0;
static uint64_t *A255055_last = NULL;
static size_t A255055_lcap = 0;
static uint64_t A255055_maxstep = 0;

static void A255055_init(void)
{
    if (A255055_st) return;
    A255055_cap = 1024;
    A255055_st = (uint32_t *)calloc(A255055_cap, sizeof(uint32_t));
    A255055_lcap = 64;
    A255055_last = (uint64_t *)calloc(A255055_lcap, sizeof(uint64_t));
    A255055_len = 1;
    A255055_maxstep = 0;
}

seq_t a(idx_t n)
{
    A255055_init();
    if (n + 2 > A255055_lcap)
    {
        size_t nc = A255055_lcap;
        while (nc < (size_t)n + 2) nc <<= 1;
        uint64_t *t = (uint64_t *)realloc(A255055_last, nc * sizeof(uint64_t));
        for (size_t i = A255055_lcap; i < nc; i++) t[i] = 0;
        A255055_last = t;
        A255055_lcap = nc;
    }
    while (A255055_maxstep <= (uint64_t)n)
    {
        if (A255055_len >= A255055_cap)
        {
            A255055_cap <<= 1;
            A255055_st = (uint32_t *)realloc(A255055_st, A255055_cap * sizeof(uint32_t));
        }
        uint64_t k = A255055_len;
        uint64_t x = k ^ (k >> 1);
        uint64_t runs = 0;
        while (x) { x &= x - 1; runs++; }
        uint32_t s = A255055_st[k - runs] + 1;
        A255055_st[k] = s;
        A255055_len++;
        if ((size_t)s >= A255055_lcap)
        {
            size_t nc = A255055_lcap << 1;
            uint64_t *t = (uint64_t *)realloc(A255055_last, nc * sizeof(uint64_t));
            for (size_t i = A255055_lcap; i < nc; i++) t[i] = 0;
            A255055_last = t;
            A255055_lcap = nc;
        }
        A255055_last[s] = k;
        if (s > A255055_maxstep) A255055_maxstep = s;
    }
    return (seq_t)A255055_last[n];
}

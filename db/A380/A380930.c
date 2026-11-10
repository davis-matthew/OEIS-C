/*
 * A380930  Numbers k such that A380845(k) > 3*k.
 * https://oeis.org/A380930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   search: a(n) is the n-th k with A380845(k) > 3*k, where
 *            A380845(k) = Sum_{d|k, popcount(d)=popcount(k)} d; computed by a
 *            segmented divisor-pair sieve with memoized terms
 * Range:     u8 none, u16 n <= 40, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 55 terms through n = 9447 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#define A380930_SEG 65536

static uint64_t *A380930_terms = NULL;
static size_t A380930_count = 0, A380930_cap = 0;
static uint64_t A380930_next_lo = 1;
static uint64_t A380930_root = 1;   /* isqrt tracker */

static inline unsigned A380930_pc(uint64_t x)
{
    x = x - ((x >> 1) & 0x5555555555555555ULL);
    x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
    x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
    return (unsigned)((x * 0x0101010101010101ULL) >> 56);
}

static void A380930_push(uint64_t k)
{
    if (A380930_count == A380930_cap) {
        size_t nc = A380930_cap ? A380930_cap * 2 : 1024;
        uint64_t *t = (uint64_t *)realloc(A380930_terms, nc * sizeof(uint64_t));
        if (!t) abort();
        A380930_terms = t;
        A380930_cap = nc;
    }
    A380930_terms[A380930_count++] = k;
}

static void A380930_segment(void)
{
    static uint64_t sums[A380930_SEG];
    static unsigned char pcs[A380930_SEG];
    uint64_t lo = A380930_next_lo;
    uint64_t hi = lo + A380930_SEG;        /* exclusive */
    uint64_t i;

    for (i = 0; i < A380930_SEG; ++i) {
        sums[i] = 0;
        pcs[i] = (unsigned char)A380930_pc(lo + i);
    }
    while ((A380930_root + 1) * (A380930_root + 1) <= hi - 1)
        ++A380930_root;
    uint64_t dmax = A380930_root;

    for (uint64_t d = 1; d <= dmax; ++d) {
        unsigned pd = A380930_pc(d);
        uint64_t q = (lo + d - 1) / d;
        if (q < d) q = d;
        uint64_t m = q * d;
        for (; m < hi; m += d, ++q) {
            uint64_t idx = m - lo;
            unsigned pm = pcs[idx];
            if (pm == pd) sums[idx] += d;
            if (q != d && A380930_pc(q) == pm) sums[idx] += q;
        }
    }

    for (i = 0; i < A380930_SEG; ++i) {
        uint64_t k = lo + i;
        if (sums[i] > 3 * k) A380930_push(k);
    }
    A380930_next_lo = hi;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while ((uint64_t)A380930_count < (uint64_t)n)
        A380930_segment();
    return (seq_t)A380930_terms[n - 1];
}

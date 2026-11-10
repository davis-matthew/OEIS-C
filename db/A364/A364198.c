/*
 * A364198  Index of first occurrence of n in A364197.
 * https://oeis.org/A364198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k with A364197(k) = n, where A364197(0) = 0 and
 *            A364197(k+1) = A364197(|k - A364197(k)^2|) + 1; computed by
 *            generating A364197 incrementally until n appears
 * Range:     u8 n <= 12, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 49 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint32_t *A364198_vals = NULL;   /* A364197 terms */
static uint64_t A364198_len = 0, A364198_cap = 0;
static uint64_t *A364198_first = NULL;  /* first occurrence per value, ~0 = unseen */
static uint64_t A364198_fcap = 0;

static void A364198_note(uint32_t v, uint64_t idx)
{
    if ((uint64_t)v >= A364198_fcap) {
        uint64_t nc = A364198_fcap ? A364198_fcap : 64;
        while (nc <= (uint64_t)v) nc *= 2;
        A364198_first = (uint64_t *)realloc(A364198_first, nc * sizeof(uint64_t));
        for (uint64_t j = A364198_fcap; j < nc; ++j) A364198_first[j] = ~(uint64_t)0;
        A364198_fcap = nc;
    }
    if (A364198_first[v] == ~(uint64_t)0) A364198_first[v] = idx;
}

seq_t a(idx_t n)
{
    if (A364198_len == 0) {
        A364198_cap = 1024;
        A364198_vals = (uint32_t *)malloc(A364198_cap * sizeof(uint32_t));
        A364198_vals[0] = 0;
        A364198_len = 1;
        A364198_note(0, 0);
    }
    for (;;) {
        if ((uint64_t)n < A364198_fcap && A364198_first[(uint64_t)n] != ~(uint64_t)0)
            return (seq_t)A364198_first[(uint64_t)n];
        uint64_t i = A364198_len;
        if (i >= A364198_cap) {
            A364198_cap *= 2;
            A364198_vals = (uint32_t *)realloc(A364198_vals, A364198_cap * sizeof(uint32_t));
        }
        uint64_t prev = i - 1;
        uint64_t v = A364198_vals[prev];
        uint64_t m2 = v * v;
        uint64_t diff = (prev >= m2) ? (prev - m2) : (m2 - prev);
        uint32_t nv = A364198_vals[diff] + 1;
        A364198_vals[i] = nv;
        A364198_len = i + 1;
        A364198_note(nv, i);
    }
}

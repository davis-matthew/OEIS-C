/*
 * A331845  Number of compositions (ordered partitions) of n into distinct
 *          cubes.
 * https://oeis.org/A331845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 440, u16 n <= 2024, u32 n <= 8280, u64 n <= 53360,
 *            u128 n <= 65536, big any n
 * Validated: standard, pass, 117 terms through n = 9833 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(s) = sum over sets S of distinct positive cubes with sum s of |S|!.
   A 0/1 knapsack over the cubes (s descending) counts the sets by size,
   c[k][s] (k <= K, since 1^3 + ... + K^3 = (K(K+1)/2)^2), and then
   a(s) = sum_k k! c[k][s], for all s <= L at once; L doubles when a larger n
   is asked. The set counts fit in 64 bits; the sums of k! times them are in
   seq_t. (The original enumerated all 2^(cube count) subsets again for every
   new n, and kept the cubes in an array of 22, overrun past n = 10648.) */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A = NULL;
static uint64_t L = 0;

static void build(uint64_t lim)
{
    unsigned K = 0;
    while ((uint64_t)(K + 1) * (K + 2) / 2 * ((uint64_t)(K + 1) * (K + 2) / 2) <= lim) ++K;
    uint64_t *c = (uint64_t *)calloc((size_t)(K + 1) * (lim + 1), sizeof *c);
    if (!c) abort();
    c[0] = 1;                                          /* c[k * (lim + 1) + s] */
    for (uint64_t i = 1; i * i * i <= lim; ++i) {
        uint64_t q = i * i * i;
        for (uint64_t s = lim; s >= q; --s)
            for (unsigned k = K; k >= 1; --k)
                c[k * (lim + 1) + s] += c[(k - 1) * (lim + 1) + s - q];
    }
    mem_free(A);
    A = mem_new(seq_t, lim + 1);
    if (!A) abort();
    seq_t f = (seq_t)1;
    for (unsigned k = 0; k <= K; ++k) {
        if (k) f = f * (seq_t)k;
        for (uint64_t s = 0; s <= lim; ++s)
            if (c[k * (lim + 1) + s]) A[s] = A[s] + f * (seq_t)c[k * (lim + 1) + s];
    }
    free(c);
    L = lim;
}

seq_t a(idx_t n)
{
    if (!L || n > L) {
        uint64_t lim = L ? 2 * L : 4096;
        while (lim < n) lim *= 2;
        build(lim);
    }
    return A[n];
}

/*
 * A379372  Sphenic numbers k such that 2*k+1 is also a sphenic number.
 * https://oeis.org/A379372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2757, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A379372 – sphenic numbers k such that 2*k+1 is also a sphenic number
   (searched sequentially with memoisation). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Test whether a 64‑bit integer is sphenic (product of exactly three distinct
   primes, each to the first power). */
static bool is_sphenic_uint64(uint64_t v)
{
    if (v < 30)                /* 2 * 3 * 5 = 30 is the smallest sphenic number */
        return false;

    size_t cnt;
    uint64_t primes[10], exps[10];

    factor_u64(v, primes, exps, &cnt);
    if (cnt != 3)
        return false;

    /* All exponents must be 1. */
    for (size_t i = 0; i < 3; ++i)
        if (exps[i] != 1)
            return false;

    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached terms */
    static idx_t size  = 0;      /* number of stored terms */
    static idx_t cap   = 0;      /* allocated capacity */
    static uint64_t cand = 2;    /* next integer to test */

    /* Grow cache as needed.  Use a simple doubling strategy. */
    if (cache == NULL) {
        cap = 256;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    while (size < n) {
        /* Search for the next valid k. */
        for (;;) {
            if (is_sphenic_uint64(cand)) {
                uint64_t twice = cand * 2 + 1;
                if (is_sphenic_uint64(twice)) {
                    /* Store the term and advance the candidate. */
                    cache[size++] = (seq_t)cand;
                    cand++;        /* move on for the next search */
                    break;         /* exit the inner loop */
                }
            }
            cand++;                /* try next integer */
        }

        /* Resize cache if it's full. */
        if (size == cap) {
            cap <<= 1;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }
    }

    return cache[n - 1];
}

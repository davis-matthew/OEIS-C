/*
 * A352382  Numbers k such that no nonzero digit of 5*k divides 5*k.
 * https://oeis.org/A352382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A352382 – finds numbers k such that no nonzero decimal digit of 5*k divides 5*k.
 * It searches sequentially, storing results in a static cache so that calls with 
 * arbitrary order of n are fast.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper that tests whether a given k satisfies the condition */
static bool valid(idx_t k)
{
    /* compute 5*k in a wider type to avoid overflow for small seq_t builds   */
    uint64_t prod = (uint64_t)k * 5ULL;

    /* iterate over decimal digits of prod */
    while (prod != 0) {
        unsigned int d = (unsigned int)(prod % 10ULL);
        prod /= 10ULL;
        if (d != 0 && ((uint64_t)5 * k) % d == 0)   /* 5*k already in prod */
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache     = NULL;
    static size_t cache_size  = 0;
    static size_t cache_cap   = 0;

    /* next even candidate k to test */
    static idx_t next_k = 2;

    /* grow cache if needed */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
    }

    /* extend cache until we have at least n terms */
    while (cache_size < n) {
        /* ensure enough space */
        if (cache_size == cache_cap) {
            cache_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            cache = tmp;
        }

        if (valid(next_k)) {
            cache[cache_size++] = (seq_t)next_k;
        }
        next_k += 2;   /* only even k can qualify */
    }

    return cache[n - 1];
}

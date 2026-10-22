/*
 * A300948  a(1) = 2, a(2) = 1, and for any n > 2 with binary expansion
 *          Sum_{i=0..k} b_i * 2^i, a(n) = Sum_{i=0..k} b_i * 2^(a(i+1)-1).
 * https://oeis.org/A300948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   For n ≥ 1, a(1) = 2, a(2) = 1 if n > 2 with binary expansion n =
 *            Σ b_i * 2^i a(n) = Σ b_i * 2^(a(i+1)-1)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (All arithmetic is on type seq_t; the exponent is cast to idx_t.) */

#include <stdlib.h>
#include "../utils/pow2.h"

/* Cached values of the sequence (index 0 unused). */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t next_missing = 1;

/* Allocate or grow the cache array if necessary. */
static void ensure_cache(idx_t need)
{
    /* Allocate or enlarge the cache array. */
    if (!cache) {
        cache_cap = (size_t)(need + 8);
        #if defined(__cplusplus)
            cache = reinterpret_cast<seq_t *>(malloc(cache_cap * sizeof(seq_t)));
        #else
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        #endif
        if (!cache) abort();
        cache[0] = (seq_t)0;   /* unused index 0 */
    } else if (need >= cache_cap) {
        size_t new_cap = cache_cap;
        while (need >= new_cap) new_cap <<= 1;   /* double until sufficient */
        #if defined(__cplusplus)
            seq_t *tmp = reinterpret_cast<seq_t *>(realloc(cache, new_cap * sizeof(seq_t)));
        #else
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        #endif
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms in increasing order. */
    for (idx_t i = next_missing; i <= need; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)2;
        } else if (i == 2) {
            cache[2] = (seq_t)1;
        } else {
            seq_t sum = 0;
            idx_t x = i;
            idx_t bit = 0;
            while (x) {
                if (x & (idx_t)1) {
                    /* exponent = a(bit+1)-1 */
                    idx_t exp = (idx_t)(cache[bit + 1] - (seq_t)1);
                    sum += pow2(exp);   /* 2^exp */
                }
                ++bit;
                x >>= 1;
            }
            cache[i] = sum;
        }
    }
    next_missing = need + 1;
}

/* Function required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* Optional guard */
    ensure_cache(n);
    return cache[n];
}

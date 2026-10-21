/*
 * A255377  Number of strings of n decimal digits that contain at least one
 *          string of exactly 7 consecutive "0" digits.
 * https://oeis.org/A255377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 15, u64 n <= 25, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0)=…=a(6)=0,  a(7)=1
   a(n) = 9 * (10^(n-8) - a(n-8) + Σ_{i=7}^{n-1} a(i))   (n ≥ 8)  */
#include <stdlib.h>   /* malloc, realloc, calloc */

static seq_t *cache = NULL;        /* stored values of a(i) */
static size_t cache_cap = 0;       /* allocated capacity (number of elements) */
static size_t cache_size = 0;      /* number of values currently initialized */
static seq_t sum_to_cur = 0;       /* Σ_{i=7}^{cur-1} a(i) for the next cur to compute */
static seq_t pow10v = 0;           /* 10^(cur-8) for the next cur to compute */

seq_t a(idx_t n)
{
    /* Lazy initialisation on the first call. */
    if (cache == NULL) {
        /* Allocate an array large enough for indices 0…7 inclusive. */
        cache_cap = 8;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        /* a(0)…a(6) are already zero.  Set a(7)=1. */
        cache[7] = (seq_t)1;
        cache_size = 8;           /* values up to index 7 are valid */
        sum_to_cur = cache[7];    /* Σ_{i=7}^{7-1} = a(7) for the next term */
        pow10v = (seq_t)1;        /* 10^(8-8) = 1  */
    }

    /* If the requested index is already known, just return it. */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* Expand the cache if the requested index is beyond the current capacity. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)   /* out of memory – fail gracefully */
            return (seq_t)0;
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute a(i) for every i from cache_size up to n (inclusive). */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        if (i <= 6) {
            /* a(0)…a(6)=0 – already zero, but set explicitly for safety. */
            cache[i] = (seq_t)0;
        } else if (i == 7) {
            cache[i] = (seq_t)1;
            sum_to_cur = cache[7];
            pow10v = (seq_t)1;  /* 10^(8-8) */
        } else { /* i ≥ 8 */
            seq_t term = pow10v - cache[i-8] + sum_to_cur;
            term = (seq_t)9 * term;   /* 9 * term */
            cache[i] = term;
            sum_to_cur += term;     /* update cumulative sum for next iteration */
            pow10v *= (seq_t)10;    /* advance 10^(i+1-8) for the next i */
        }
    }

    cache_size = (size_t)n + 1;   /* now we have values up to n inclusive */
    return cache[(size_t)n];
}

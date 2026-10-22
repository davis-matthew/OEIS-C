/*
 * A307557  Number of Motzkin meanders of length n with no level steps at odd
 *          level.
 * https://oeis.org/A307557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   (n+1)*a(n) + (-n-2)*a(n-1) + (-5*n+3)*a(n-2) + (n+4)*a(n-3) + 2*(2*n-5)*a(n-4) = 0
   Initial values:
   a(0) = 1,  a(1) = 2,  a(2) = 4,  a(3) = 9
   For n >= 4 the recurrence yields the sequence. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* first non‑computed index */

    /* initialise cache with first 4 values */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* out‑of‑memory, unlikely */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)9;
        cache_len = 4;                    /* elements 0..3 are known */
    }

    /* return if already computed */
    if (n < cache_len) return cache[n];

    /* enlarge cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;   /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* out‑of‑memory, unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term1 = ((seq_t)(i + 2)) * cache[i - 1];
        seq_t term2 = ((seq_t)(5 * i - 3)) * cache[i - 2];
        seq_t term3 = ((seq_t)(i + 4)) * cache[i - 3];
        seq_t term4 = ((seq_t)(2 * (2 * i - 5))) * cache[i - 4];

        seq_t numerator = term1 + term2;
        numerator -= term3;
        numerator -= term4;

        cache[i] = numerator / ((seq_t)(i + 1));
    }

    cache_len = n + 1;
    return cache[n];
}

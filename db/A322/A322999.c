/*
 * A322999  The successive approximations up to 2^n for 2-adic integer
 *          9^(1/3).
 * https://oeis.org/A322999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A322999: successive approximations to 9^(1/3) in the 2-adics.
   Recurrence:
     a(0) = 0
     for n > 0,
        a(n) = a(n-1)               if a(n-1)^3 - 9 is divisible by 2^n
        a(n) = a(n-1) + 2^(n-1)     otherwise
   a(n) is the unique integer in [0,2^n-1] with a(n)^3 ≡ 9 (mod 2^n). */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t) */

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Resize cache if necessary. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = ((size_t)n + 1) + 1024;   /* small extra room */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – treat as empty */
            tmp = cache;
            cache_cap = 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If we already have the term, return it. */
    if (n < cache_len) return cache[n];

    /* Initialise first element if cache empty. */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    /* Compute new terms sequentially, caching each. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t t    = prev * prev * prev - (seq_t)9;   /* a(n-1)^3 - 9 */
        seq_t pow_i   = pow2(i);                      /* 2^i */
        seq_t mask = pow_i - (seq_t)1;                /* 2^i - 1 */

        if ((t & mask) == 0) {
            cache[i] = prev;                         /* keep same value */
        } else {
            cache[i] = prev + pow2(i - 1);           /* add 2^(i-1) */
        }
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}

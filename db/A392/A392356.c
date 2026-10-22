/*
 * A392356  a(n) = Sum_{k=0..floor(5*n/9)} binomial(k,5*n-9*k).
 * https://oeis.org/A392356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..⌊5n/9⌋} binomial(k,5n‑9k)
 * Range:     u8 n <= 20, u16 n <= 35, u32 n <= 67, u64 n <= 127,
 *            u128 n <= 249, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (valid for n ≥ 11):          */
/* a(n) = 5·a(n‑2) - 10·a(n‑4) + 10·a(n‑6) - 5·a(n‑8) + a(n‑9) + a(n‑10) */
/* Initial values (n = 0 … 10):            */
/* 1,0,1,0,1,0,1,0,1,1,1 */

#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;      /* array of computed terms */
    static idx_t  cache_size = 0;    /* number of entries currently computed (cache[0..size-1]) */
    static idx_t  cache_cap  = 0;    /* allocated capacity of the array */

    /* Initial terms for n = 0 … 10 */
    const seq_t init[11] = {
        [0] = (seq_t)1,
        [1] = (seq_t)0,
        [2] = (seq_t)1,
        [3] = (seq_t)0,
        [4] = (seq_t)1,
        [5] = (seq_t)0,
        [6] = (seq_t)1,
        [7] = (seq_t)0,
        [8] = (seq_t)1,
        [9] = (seq_t)1,
        [10] = (seq_t)1
    };

    /* Create cache with the initial 11 values if not yet done */
    if (cache == NULL) {
        cache_cap  = 11;
        cache_size = 11;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();    /* allocation failure */
        for (idx_t i = 0; i < cache_cap; ++i) {
            cache[i] = init[i];
        }
    }

    /* If the requested value is already cached, return it */
    if (n < cache_size) {
        return cache[n];
    }

    /* Resize cache if necessary */
    if ((idx_t)(n + 1) > cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms from cache_size up to n using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i >= 11) {
            /* Positive part: 5*a(i‑2) + 10*a(i‑6) + a(i‑9) + a(i‑10) */
            seq_t pos = (seq_t)5 * cache[i - 2]
                      + (seq_t)10 * cache[i - 6]
                      + cache[i - 9] + cache[i - 10];

            /* Negative part: 10*a(i‑4) + 5*a(i‑8) */
            seq_t neg = (seq_t)10 * cache[i - 4]
                      + (seq_t)5  * cache[i - 8];

            cache[i] = pos - neg;   /* guaranteed non‑negative */
        } else {
            /* For i < 11 the values are already set (should not occur) */
            cache[i] = init[i];
        }
    }

    cache_size = n + 1;
    return cache[n];
}

/*
 * A280240  Expansion of 1/(1 - Sum_{k>=1} x^(k+floor(1/2+sqrt(k)))).
 * https://oeis.org/A280240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for n>0, a(n)=∑_{k=1..n, k not a perfect square} a(n−k). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* Static cache for computed values. */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last_computed = 0;      /* largest i with cache[i] valid */

    /* Static array indicating whether an index is a perfect square. */
    static bool *is_square = NULL;
    static size_t sq_cap = 0;            /* capacity of is_square array */

    /* Ensure the main cache can hold indices up to n. */
    if (!cache || (idx_t)n >= cap) {
        ensure_cache(n, &cache, &cap);
    }

    /* Ensure the square-flag array can hold indices up to n. */
    if ((idx_t)(n + 1) > sq_cap) {
        bool *tmp = (bool *)realloc(is_square, (n + 1) * sizeof(bool));
        if (!tmp) exit(EXIT_FAILURE);    /* allocation failed */
        is_square = tmp;
        /* Initialise new slots to false. */
        for (idx_t i = sq_cap; i <= n; ++i) {
            is_square[i] = false;
        }
        /* Mark perfect squares up to n. */
        for (idx_t i = 1; (idx_t)(i * i) <= n; ++i) {
            is_square[i * i] = true;
        }
        sq_cap = n + 1;
    }

    /* Initialise a(0) if this is the first invocation. */
    if (last_computed == 0 && cache[0] == 0) {
        cache[0] = (seq_t)1;
    }

    /* Compute values from last_computed+1 up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            if (!is_square[k]) {
                sum += cache[i - k];
            }
        }
        cache[i] = sum;
    }
    last_computed = n;
    return cache[n];
}

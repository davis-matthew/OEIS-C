/*
 * A330547  a(1)=2; thereafter a(n) = a(n-1) +
 *          (-1)^(n+1)*(prime(n)-prime(n-1)) (where prime(k) denotes the k-th
 *          prime).
 * https://oeis.org/A330547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1)=2; a(n)=a(n−1)+(-1)^(n+1)*(prime(n)-prime(n-1)) for n>1
 *            where prime(k) is the k‑th prime
 * Range:     i8 n <= 779, i16 n <= 9615, i32 n <= 9615, i64 n <= 9615,
 *            i128 n <= 9615, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"   /* provides uint32_t get_prime(idx_t k) */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = 0;

    /* ensure cache big enough for index n (1‑based) */
    if (n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;          /* indices 0..n */
        if (cache) {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        } else {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        }
        cache_cap = new_cap;
    }

    /* initialise first term */
    if (last_computed == 0) {
        cache[1] = (seq_t)2;
        last_computed = 1;
    }

    /* compute any missing values up to n */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            uint32_t p_i   = get_prime(i);        /* ith prime  */
            uint32_t p_prev = get_prime(i - 1);    /* (i-1)th prime */
            seq_t diff = (seq_t)(p_i - p_prev);    /* always >=0 */

            /* sign: (-1)^(i+1): + for odd i, - for even i */
            if ((i & 1U) == 0U) diff = -diff;

            cache[i] = cache[i - 1] + diff;
        }
        last_computed = n;
    }

    return cache[n];
}

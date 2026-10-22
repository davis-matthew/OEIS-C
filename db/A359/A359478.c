/*
 * A359478  a(1) = 1; a(n) = -Sum_{k=2..n} k * a(floor(n/k)).
 * https://oeis.org/A359478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(1) = 1; for n > 1, a(n) = -Σ_{k=2}^{n} k * a(⌊n/k⌋)
 * Range:     i8 n <= 64, i16 n <= 2193, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises values in a static cache so that calls
   may arrive in any order. It works for any build where seq_t is an
   arbitrary‑precision integer class or a fixed size integer type. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Per‑call cache of already computed terms. */
    static seq_t *cache = NULL;
    static bool *computed = NULL;
    static size_t cap = 0;      /* capacity of the cache arrays */

    /* Grow cache if necessary. */
    if (n + 1 > cap) {
        size_t new_cap = n + 1;
        seq_t *tmp_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        bool *tmp_comp  = (bool *)realloc(computed, new_cap * sizeof(bool));
        if (!tmp_cache || !tmp_comp) abort();   /* out of memory */

        cache    = tmp_cache;
        computed = tmp_comp;
        for (size_t i = cap; i < new_cap; ++i)
            computed[i] = false;
        cap = new_cap;
    }

    /* Return cached value if already known. */
    if (computed[n])
        return cache[n];

    seq_t result;
    if (n == 1) {
        result = 1;
    } else {
        seq_t sum = 0;
        for (idx_t k = 2; k <= n; ++k) {
            idx_t m = n / k;            /* floor division */
            seq_t aj = a(m);             /* recursive call, memoised */
            sum += (seq_t)k * aj;
        }
        result = -sum;
    }

    /* Cache and return. */
    cache[n] = result;
    computed[n] = true;
    return result;
}

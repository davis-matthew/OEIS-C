/*
 * A351905  Expansion of e.g.f. exp(x * (1 - x^3)).
 * https://oeis.org/A351905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1) - 24 * binomial(n-1,3) * a(n-4) with initial
 *            values a(0)=a(1)=a(2)=a(3)=1 (OEIS A351905: Expansion of e.g.f.
 *            exp(x*(1-x^3)))
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 22, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>    /* size_t */
#include <stdlib.h>   /* malloc, realloc */

/*  seq_t and idx_t are defined by the build environment.
    idx_t is an unsigned 64‑bit integer, seq_t may be an
    integer type or an arbitrary‑precision class.  The function
    below is valid for both C11 and C++17. */
seq_t a(idx_t n)
{
    /* Static cache to allow calls in any order.  The cache
       holds all values from 0 up to the largest index computed
       so far. */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;      /* number of valid entries in cache */

    /* Extend the cache to cover n, if necessary. */
    if (n >= cap) {
        size_t new_cap = n + 1;
        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = reinterpret_cast<seq_t*>(realloc(cache,
                                                      new_cap * sizeof(seq_t)));
#else
        new_cache = (seq_t*)realloc(cache,
                                    new_cap * sizeof(seq_t));
#endif
        if (!new_cache) {
            /* Allocation failed; return a zero value as a
               simple failure indicator (dims. to the caller). */
            return (seq_t)0;
        }
        cache = new_cache;

        /* Compute values that have just become available. */
        for (size_t i = cap; i < new_cap; ++i) {
            if (i <= 3) {
                cache[i] = (seq_t)1;
            } else {
                /* bin = binomial(i-1,3) = (i-1)(i-2)(i-3)/6  */
                seq_t bin = (seq_t)(i - 1) * (seq_t)(i - 2) *
                            (seq_t)(i - 3) / (seq_t)6;
                cache[i] = cache[i - 1] - (seq_t)24 * bin * cache[i - 4];
            }
        }
        cap = new_cap;
    }
    return cache[n];
}

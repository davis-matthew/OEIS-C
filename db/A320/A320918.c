/*
 * A320918  Sum of n-th powers of the roots of x^3 + 9*x^2 + 20*x - 1.
 * https://oeis.org/A320918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 3, a(1) = -9, a(2) = 41, a(n) = -9*a(n-1) - 20*a(n-2) +
 *            a(n-3) for n > 2
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 13, i64 n <= 28, i128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/*  Cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of elements currently stored */

/*  Compute a(n) using the linear recurrence and memoise results  */
seq_t a(idx_t n)
{
    /*  Ensure the cache is big enough  */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache =
            (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();              /* allocation failure */

        cache     = new_cache;
        cache_len = new_len;

        /*  Initialise the first three terms if not already done  */
        if (cache_len == 0 || cache_len < 3) {
            /* Allocate space for at least three elements */
            size_t min_len = (cache_len < 3) ? 3 : cache_len;
            seq_t *tmp =
                (seq_t *)realloc(cache, min_len * sizeof(seq_t));
            if (!tmp) abort();
            cache    = tmp;
            cache_len = min_len;
        }
        if (cache_len >= 3) {
            cache[0] = (seq_t)3;
            cache[1] = (seq_t)-9;
            cache[2] = (seq_t)41;
        }

        /*  Compute missing terms up to n  */
        for (size_t i = 3; i <= (size_t)n; ++i) {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            cache[i] = - (seq_t)9 * a1 - (seq_t)20 * a2 + a3;
        }
    }

    return cache[(size_t)n];
}

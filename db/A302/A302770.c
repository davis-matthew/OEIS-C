/*
 * A302770  a(n) = (4*n-2)*((n-1)*a(n-1) + ((n-2)!)^2), with a(1) = 0, n > 1.
 * https://oeis.org/A302770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n-2)*((n-1)*a(n-1) + ((n-2)!)^2), a(1) = 0
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* the function must be exported with external linkage */
seq_t a(idx_t n)
{
    /* cache for already computed values */
    static seq_t *cache = NULL;      /* 1‑indexed array, cache[1] = 0 */
    static size_t cache_size = 0;    /* number of allocated slots (size) */
    static idx_t  last = 1;          /* highest index that has been computed */
    static seq_t  last_fact = 1;     /* factorial of (last-1)! after last step */

    if (n == 0) {                     /* not part of the OEIS offset */
        return (seq_t)0;
    }

    if (n == 1) {
        return (seq_t)0;
    }

    /* expand cache if necessary */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) {
            /* memory allocation failed – abort as per requirements */
            abort();
        }
        cache = new_cache;
        cache_size = new_size;
    }

    /* if we already computed the requested value, return it */
    if (n <= last) {
        return cache[n];
    }

    /* start computation from the next uncomputed index */
    seq_t fact = last_fact;               /* factorial of (i-2)! for the first i */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* factorial squared */
        seq_t sq = fact * fact;

        /* recurrence: a(i) = (4*i-2) * ((i-1)*a(i-1) + sq) */
        seq_t term = (seq_t)(4 * i - 2)
                     * ((seq_t)(i - 1) * cache[i - 1] + sq);
        cache[i] = term;

        /* update factorial for next step: factorial((i+1)-2)! = factorial(i-1)! */
        fact = fact * (seq_t)(i - 1);
    }

    /* remember the new state for subsequent calls */
    last = n;
    last_fact = fact;

    return cache[n];
}

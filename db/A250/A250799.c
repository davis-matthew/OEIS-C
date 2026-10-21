/*
 * A250799  Number of (2+1) X (n+1) 0..1 arrays with nondecreasing
 *          min(x(i,j),x(i,j-1)) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   OEIS A250799 a(n) = 4*a(n-1) - 4*a(n-2) - a(n-3) + 2*a(n-4) (n ≥
 *            5) a(1) = 24, a(2) = 66, a(3) = 162, a(4) = 376 (Defined for n ≥
 *            1. For n < 1 behaviour is irrelevant.)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* cache[0] is unused to keep indices 1‑based */
    static seq_t *cache     = NULL;   /* array of terms (index 0 unused) */
    static size_t cache_cap = 0;      /* current allocated size   */
    static idx_t   max_comp  = 0;     /* largest n that has been computed */

    /* Ensure cache has room for indices up to n */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        /* We need space for the first four terms when we grow for the
           first time or when cache is too small to hold them. */
        if (new_cap < 5) new_cap = 5;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        size_t old_cap = cache_cap;
        cache_cap = new_cap;

        /* First time the cache is grown large enough to store the known
           initial terms: set them.  This also covers the case where the
           cache starts as NULL. */
        if (old_cap < 5 && cache_cap >= 5) {
            cache[1] = (seq_t)24;
            cache[2] = (seq_t)66;
            cache[3] = (seq_t)162;
            cache[4] = (seq_t)376;
        }

        /* In any case we know that the first four terms exist now,
           so mark them as already computed. */
        if (max_comp < 4) max_comp = 4;
    }

    /* For n ≤ 4 we can return the stored constants.  (If cache was
       grown without exceeding 4, the constants are already set above.) */
    if (n <= 4) return cache[n];

    /* Compute new terms only if necessary. */
    if (max_comp < n) {
        /* Ensure we start from the first position where the recurrence
           is valid. */
        idx_t start = max_comp + 1;
        if (start < 5) start = 5;

        for (idx_t k = start; k <= n; ++k) {
            seq_t t = cache[k-1] * (seq_t)4;
            t = t - (cache[k-2] * (seq_t)4);
            t = t - cache[k-3];
            t = t + (cache[k-4] * (seq_t)2);
            cache[k] = t;
        }
        max_comp = n;
    }

    return cache[n];
}

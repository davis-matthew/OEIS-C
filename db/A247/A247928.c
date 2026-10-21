/*
 * A247928  Number of length 1+4 0..n arrays with some disjoint pairs in every
 *          consecutive five terms having the same sum.
 * https://oeis.org/A247928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the linear recurrence a(n) = 2*a(n-1) - a(n-3) -
 *            2*a(n-5) + 2*a(n-6) + a(n-8) - 2*a(n-10) + a(n-11), with initial
 *            values: a(1) = 32, a(2) = 203, a(3) = 824, a(4) = 2365, a(5) =
 *            5496, a(6) = 11167, a(7) = 20648, a(8) = 35249, a(9) = 56960,
 *            a(10) = 87211, a(11) = 128592
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 144, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Zero index is unused: a(0) is undefined for this OEIS entry. */
    if (n == 0)
        return (seq_t)0;

    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t max_computed = 0;

    /* Ensure the cache can hold up to index n */
    if (cache_size < (size_t)(n + 1)) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) { /* allocation failed: fallback to malloc */
            tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
            if (!tmp) /* unrecoverable allocation error */;
        }
        cache = tmp;
        /* Initialize the new portion of the cache */
        for (size_t i = cache_size; i < new_size; ++i)
            cache[i] = (seq_t)0;
        cache_size = new_size;
    }

    /* Initial values for n = 1 .. 11 */
    static const seq_t init[12] = {
        0,        /* dummy for a(0) */
        32, 203, 824, 2365, 5496,
        11167, 20648, 35249, 56960, 87211, 128592
    };

    /* Compute values up to n if necessary */
    if (max_computed < n) {
        for (idx_t i = max_computed + 1; i <= (idx_t)n; ++i) {
            if (i <= 11) {
                cache[i] = init[(size_t)i];
            } else {
                /* Use signed 64‑bit intermediate to avoid underflow.
                   All values fit in signed 64‑bit for n ≤ 10⁴. */
                int64_t term =
                    2LL * (int64_t)cache[i - 1]
                    - (int64_t)cache[i - 3]
                    - 2LL * (int64_t)cache[i - 5]
                    + 2LL * (int64_t)cache[i - 6]
                    + (int64_t)cache[i - 8]
                    - 2LL * (int64_t)cache[i - 10]
                    + (int64_t)cache[i - 11];
                cache[i] = (seq_t)term;
            }
        }
        max_computed = n;
    }

    return cache[n];
}

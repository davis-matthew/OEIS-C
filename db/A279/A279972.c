/*
 * A279972  Number of n X 3 0..1 arrays with no element equal to a strict
 *          majority of its horizontal and antidiagonal neighbors, with the
 *          exception of exactly two elements, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A279972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) - 90*a(n-2) + 281*a(n-3) - 510*a(n-4) +
 *            585*a(n-5) - 437*a(n-6) + 210*a(n-7) - 60*a(n-8) + 8*a(n-9), for
 *            n > 10. Initial values a(1) .. a(10) are given below
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial values for 1 <= n <= 10 (a(0) unused) */
    static const seq_t init[11] = {
        0ULL,          /* dummy 0 index */
        0ULL,          /* a(1) */
        9ULL,          /* a(2) */
        50ULL,         /* a(3) */
        296ULL,        /* a(4) */
        1650ULL,       /* a(5) */
        8666ULL,       /* a(6) */
        43543ULL,      /* a(7) */
        211650ULL,     /* a(8) */
        1002602ULL,    /* a(9) */
        4652327ULL     /* a(10) */
    };

    /* Return immediately for the first ten terms */
    if (n <= 10)
        return init[n];

    /* static memoisation cache (1‑based indexing) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* last index stored in cache */

    /* grow the cache if needed */
    if (cache_cap < n) {
        /* allocate or resize the buffer */
        seq_t *newp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        cache = newp;

        /* fill in any missing terms up to n */
        for (idx_t i = cache_cap + 1; i <= n; ++i) {
            if (i <= 10) {
                /* copy the known initial values */
                cache[i] = init[i];
            } else {
                /* apply the recurrence */
                cache[i] = (seq_t)15 * cache[i - 1]
                         - (seq_t)90 * cache[i - 2]
                         + (seq_t)281 * cache[i - 3]
                         - (seq_t)510 * cache[i - 4]
                         + (seq_t)585 * cache[i - 5]
                         - (seq_t)437 * cache[i - 6]
                         + (seq_t)210 * cache[i - 7]
                         - (seq_t)60  * cache[i - 8]
                         + (seq_t)8   * cache[i - 9];
            }
        }
        cache_cap = n;
    }

    return cache[n];
}

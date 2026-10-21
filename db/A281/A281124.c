/*
 * A281124  Number of n X 2 0..2 arrays with no element equal to more than one
 *          of its horizontal, vertical and antidiagonal neighbors, with the
 *          exception of exactly one element, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A281124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 8*a(n-2) - 30*a(n-3) - 24*a(n-4) - 8*a(n-5) -
 *            a(n-6); Initial values: a(1)=0, a(2)=2, a(3)=28, a(4)=208,
 *            a(5)=1364, a(6)=8354
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache      = NULL; /* cache[1]..cache[n] hold computed terms */
    static idx_t  cached_upto = 0;   /* highest index computed so far */

    /* official initial terms (1‑based indices, index 0 unused) */
    static const seq_t init[7] = {0, 0, 2, 28, 208, 1364, 8354};

    /* trivial bounds check – not strictly necessary for OEIS usage */
    if (n == 0) return 0;

    /* allocate or grow cache if needed */
    if (cache == NULL || n > cached_upto) {
        /* allocate at least up to n */
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failure – return 0 as a safe fallback */
            return 0;
        }
        cache = new_cache;
        if (cached_upto == 0) {
            /* initialise known seeds */
            for (idx_t i = 1; i <= 6 && i <= n; ++i) {
                cache[i] = init[i];
            }
            cached_upto = 6 < n ? 6 : n;
        }
    }

    /* compute missing terms up to n */
    for (idx_t i = cached_upto + 1; i <= n; ++i) {
        if (i <= 6) {
            cache[i] = init[i];
        } else {
            cache[i] = 8 * cache[i - 1]
                     - 8 * cache[i - 2]
                     - 30 * cache[i - 3]
                     - 24 * cache[i - 4]
                     - 8 * cache[i - 5]
                     - cache[i - 6];
        }
    }

    cached_upto = n;
    return cache[n];
}

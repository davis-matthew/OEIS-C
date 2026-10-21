/*
 * A283585  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 654", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A283585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = c_n + a(n-1) + 100000000*a(n-8) – 100000000*a(n-9) *
 *            where c_n = * 1, 10, 100, 990, 10000, 99000, 999900, 9901010, 0,
 *            * –10000000, –1000000, 10100000, –10000 for n = 0 … 12, * 0
 *            otherwise. a(k) = 0 for k < 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const int64_t _c_vals[13] = {
    1, /* n = 0 */
    10, /* n = 1 */
    100, /* n = 2 */
    990, /* n = 3 */
    10000, /* n = 4 */
    99000, /* n = 5 */
    999900, /* n = 6 */
    9901010, /* n = 7 */
    0, /* n = 8 */
   -10000000, /* n = 9 */
   -1000000, /* n = 10 */
   10100000, /* n = 11 */
   -10000 /* n = 12 */
};

/* Return the coefficient c_n for a given n, 0 <= n <= 12.
 * For n > 12 the coefficient is 0. */
static int64_t _c_of(idx_t n)
{
    return (n < 13) ? _c_vals[(size_t)n] : 0;
}

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms                 */
    static seq_t *cache = NULL;      /* array of a(0)..a(cache_len-1)   */
    static idx_t cache_len = 0;      /* number of values currently stored */
    static idx_t cache_cap = 0;      /* allocated capacity of cache      */

    /* Ensure the cache can hold up to n (inclusive).              */
    if (cache_len <= n) {
        /* Allocate or grow the cache as necessary.                 */
        if (cache == NULL) {
            /* Start with capacity for at least 13 terms so that
               we can store the first 13 results without further
               reallocation.                                         */
            cache_cap = (n < 13) ? 13 : (size_t)n + 1;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (!cache) return (seq_t)0; /* allocation failure */
            cache_len = 0;
        } else if ((size_t)n + 1 > cache_cap) {
            /* Grow the cache to at least the needed size, using a
               multiplicative growth to keep time bounded.        */
            while (cache_cap <= (size_t)n + 1)
                cache_cap <<= 1;
            seq_t *new_cache = (seq_t *)
                realloc(cache, cache_cap * sizeof(seq_t));
            if (!new_cache) return (seq_t)0; /* allocation failure */
            cache = new_cache;
        }

        /* Recurrence constants.                                   */
        const seq_t K = (seq_t)100000000; /* 10^8 */

        /* Compute terms from cache_len up to n (inclusive).        */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val = 0;

            /* a(i-1) term. */
            if (i > 0) val += cache[i - 1];

            /* 100000000 * a(i-8) term. */
            if (i >= 8) val += K * cache[i - 8];

            /* –100000000 * a(i-9) term. */
            if (i >= 9) val -= K * cache[i - 9];

            /* Add the coefficient c_i (may be negative).          */
            int64_t c = _c_of(i);
            if (c >= 0) {
                val += (seq_t)c;
            } else {
                /* c is negative; perform subtraction.             */
                val -= (seq_t)(-c);
            }

            cache[i] = val;
        }

        cache_len = (size_t)n + 1;
    }

    return cache[n];
}

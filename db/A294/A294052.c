/*
 * A294052  Number of set partitions of [n] such that the maximal absolute
 *          difference between consecutive elements within a block equals
 *          three.
 * https://oeis.org/A294052
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of set partitions of [n] such that the maximal
 *            absolute difference between consecutive elements within a block
 *            equals 3. The sequence satisfies the linear recurrence derived
 *            from the rational generating function A(x) = (x-2)x^4 /
 *            ((x-1)(x^2-3x+1)(x^3-x^2-3x+1)) : a(n) = 7 a(n-1) - 1...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation does not hard‑code terms; it computes all values up to the
   requested index on demand and memoises the results. */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoised coefficients */
static idx_t  cache_len = 0;  /* number of computed indices (inclusive) */

/* Return a[n] for n ≥ 4; values for n < 4 are defined as 0. */
seq_t a(idx_t n)
{
    /* Grow cache if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure, return 0 as fallback */
        cache = tmp;

        /* Compute coefficients from the last known index up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = 0;

            if (i >= 1) sum += (seq_t)7   * cache[i-1];
            if (i >= 3) sum += (seq_t)8   * cache[i-3];
            if (i >= 4) sum += (seq_t)5   * cache[i-4];
            if (i >= 6) sum +=           cache[i-6];

            if (i >= 2) sum -= (seq_t)15  * cache[i-2];
            if (i >= 5) sum -= (seq_t)5   * cache[i-5];

            if (i == 4) sum += (seq_t)2;  /* subtract r(4) = -2 */
            else if (i == 5) sum -= (seq_t)1; /* subtract r(5) = 1 */

            cache[i] = sum;
        }
        cache_len = new_len;
    }
    return cache[n];
}

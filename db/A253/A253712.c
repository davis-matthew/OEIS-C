/*
 * A253712  Second partial sums of 12th powers (A008456).
 * https://oeis.org/A253712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + n^12, a(1) = 1, a(2) = 4098
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to store previously computed terms.
       cache[0] = a(1), cache[1] = a(2), ...  */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */

    /* Initialize cache with first two terms if necessary */
    if (cache_len < 2) {
        size_t init_len = 2;
        cache = (seq_t *)realloc(cache, init_len * sizeof(seq_t));
        cache_len = init_len;
        cache[0] = (seq_t)1;     /* a(1) */
        cache[1] = (seq_t)4098;  /* a(2) */
    }

    /* If the requested index is already cached, return it */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* Extend the cache to hold up to index n */
    size_t old_len = cache_len;
    size_t new_len = n;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    cache_len = new_len;

    /* Compute missing terms iteratively using the recurrence */
    for (idx_t i = old_len + 1; i <= n; ++i) {
        /* Compute i^12 */
        seq_t base = (seq_t)i;
        seq_t pow = base;
        for (int k = 1; k < 12; ++k) {
            pow *= base;
        }

        /* a(i) = 2*a(i-1) - a(i-2) + i^12 */
        seq_t prev1 = cache[i - 2];     /* a(i-1) */
        seq_t prev2 = cache[i - 3];     /* a(i-2) */
        seq_t val = prev1 * 2;
        val -= prev2;
        val += pow;

        cache[i - 1] = val;
    }

    return cache[n - 1];
}

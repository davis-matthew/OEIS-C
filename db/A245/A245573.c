/*
 * A245573  Minimal coin changing sequence for denominations 1, 2, 5 and 10
 *          cents.
 * https://oeis.org/A245573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-10) - a(n-11) for n > 10; a(0) = 0, a(1) =
 *            1, a(2) = 1, a(3) = 2, a(4) = 2, a(5) = 1, a(6) = 2, a(7) = 2,
 *            a(8) = 3, a(9) = 3, a(10) = 1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  Cache to store already computed terms.  */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* current capacity of cache   */
    static idx_t last_computed = 10;      /* largest index computed so far */

    /* Initial values for indices 0 … 10 */
    static const seq_t init[11] = {0,1,1,2,2,1,2,2,3,3,1};

    /* Direct return for the base cases. */
    if (n <= 10)
        return init[n];

    /* Grow the cache if needed.  */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
            if (cache == NULL)         /* insufficient memory */
                return 0;
            for (idx_t i = 0; i <= 10; ++i)
                cache[i] = init[i];
        } else {
            seq_t *tmp = (seq_t *)realloc(cache,
                                          new_size * sizeof(seq_t));
            if (tmp == NULL)           /* insufficient memory */
                return 0;
            cache = tmp;
            /* Newly allocated elements are uninitialised; they will be
               computed in the loop below. */
        }
        cache_size = new_size;
    }

    /* Compute any missing values up to n.  */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* a(i) = a(i-1) + a(i-10) - a(i-11) */
        seq_t val = cache[i - 1];
        val += cache[i - 10];
        val -= cache[i - 11];
        cache[i] = val;
    }
    last_computed = n;

    return cache[n];
}

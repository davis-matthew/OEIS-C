/*
 * A259858  A bisection of A002083.
 * https://oeis.org/A259858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 1, a(2) = 3; a(n) = 4*a(n-1) + ((-1)^(n-1)-3)/2
 *            * a(floor((n-1)/2)) for n ≥ 3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values; index type is idx_t */
    static seq_t *cache   = NULL;
    static idx_t cache_len = 0;   /* number of elements currently stored */

    /* If the cache is not large enough grow it */
    if (n + 1 > cache_len) {
        /* remember the old length so we know from where to start computing */
        idx_t old_len = cache_len;

        /* realloc to the required size (n+1 elements) */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (tmp == NULL && n + 1 > 0) {
            /* allocation failure – abort the program */
            abort();
        }
        cache     = tmp;
        cache_len = n + 1;

        /* Compute all missing terms up to n */
        for (idx_t i = old_len; i <= n; ++i) {
            if (i == 0 || i == 1) {
                cache[i] = (seq_t)1;
            } else if (i == 2) {
                cache[i] = (seq_t)3;
            } else {
                seq_t prev = cache[i - 1];
                seq_t val  = prev * (seq_t)4;   /* 4*a(n-1) */

                /* floor((i-1)/2) */
                idx_t half_idx = (i - 1) / 2;
                seq_t half_val = cache[half_idx];

                /* coefficient: -1 for odd i, -2 for even i           */
                if ((i & 1) == 0) {              /* i even -> coeff = -2 */
                    val -= half_val * (seq_t)2;
                } else {                         /* i odd  -> coeff = -1 */
                    val -= half_val;
                }

                cache[i] = val;
            }
        }
    }

    /* The cache is now guaranteed to contain a(n) */
    return cache[n];
}

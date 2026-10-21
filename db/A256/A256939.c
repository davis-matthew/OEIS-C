/*
 * A256939  Expansion of g.f.:
 *          (1-4*z-sqrt(1-8*z+12*z^2+8*z^3-4*z^4))/(2*z^2(1-z)).
 * https://oeis.org/A256939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   (n+2)*a(n) = 3*(3*n+2)*a(n-1) * - 4*(5*n-2)*a(n-2) * +
 *            4*(n+2)*a(n-3) * + 12*(n-3)*a(n-4) * - 4*(n-4)*a(n-5) * with
 *            initial values a(0)=1, a(1)=3, a(2)=13, a(3)=57, a(4)=257
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Constant initial values */
    static const seq_t init[5] = { (seq_t)1, (seq_t)3, (seq_t)13,
                                   (seq_t)57, (seq_t)257 };

    /* Grow the cache if necessary */
    if (n >= cache_size) {
        /* Allocate new storage */
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache)
            abort();                   /* allocation failed */
        cache = new_cache;

        /* Compute missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i < 5) {
                /* Direct initial values */
                cache[i] = init[i];
            } else {
                /* Recurrence computation */
                seq_t term1 = (seq_t)3 *
                              ((seq_t)(3 * i + 2) * cache[i - 1]);

                seq_t term2 = (seq_t)4 *
                              ((seq_t)(5 * i - 2) * cache[i - 2]);

                seq_t term3 = (seq_t)4 *
                              ((seq_t)(i + 2) * cache[i - 3]);

                seq_t term4 = (seq_t)12 *
                              ((seq_t)(i - 3) * cache[i - 4]);

                seq_t term5 = (seq_t)4 *
                              ((seq_t)(i - 4) * cache[i - 5]);

                /* Numerator of the recurrence */
                seq_t numer = term1 - term2 + term3 + term4 - term5;

                /* Denominator (n + 2) is guaranteed to divide the numerator */
                cache[i] = numer / (seq_t)(i + 2);
            }
        }

        cache_size = n + 1;
    }

    return cache[n];
}

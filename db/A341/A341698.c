/*
 * A341698  a(1) = a(2) = 1; a(n+1) = -Sum_{d|n, d < n} a(n/d) * a(d).
 * https://oeis.org/A341698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = a(2) = 1; a(n+1) = -∑_{d|n, d < n} a(n/d) * a(d) (OEIS
 *            A341698)
 * Range:     i8 n <= 63, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0; /* number of computed terms */

    if (n == 0) return (seq_t)0;   /* out of bounds, but harmless */

    /* Extend cache up to requested n */
    if ((size_t)n > cache_len) {
        size_t new_len = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed – return 0 */

        cache = tmp;

        /* Compute new terms */
        for (idx_t i = (idx_t)(cache_len + 1); i <= n; ++i) {
            size_t idx_i = (size_t)(i - 1);

            if (i == 1 || i == 2) {
                cache[idx_i] = (seq_t)1;
                continue;
            }

            /* Compute a(i) from the recurrence using m = i-1 */
            idx_t m = i - 1;
            seq_t sum = (seq_t)0;

            /* Enumerate divisors d of m, with 1 <= d <= sqrt(m) */
            for (idx_t d = 1; d * d <= m; ++d) {
                if (m % d == 0) {
                    idx_t e = m / d;   /* complementary divisor */

                    /* Term for divisor d (always proper, d < m) */
                    sum += cache[(size_t)((m / d) - 1)] * cache[(size_t)(d - 1)];

                    /* Term for complementary divisor e, if it is proper and distinct */
                    if (e != d && e != m) {
                        sum += cache[(size_t)((m / e) - 1)] * cache[(size_t)(e - 1)];
                    }
                }
            }

            cache[idx_i] = -sum;
        }

        cache_len = new_len;
    }

    return cache[(size_t)(n - 1)];
}

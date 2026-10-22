/*
 * A374058  Expansion of Product_{k>=1} (1 - x^(3*k-2)) * (1 - x^(3*k)).
 * https://oeis.org/A374058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the coefficient of x^n in Π_{k≥1}(1 - x^(3k-2))·(1 -
 *            x^(3k)). The product is equivalent to Π_{m≥1, m mod 3 ≠ 2}(1 -
 *            x^m). The implementation multiplies the generating function by
 *            all factors (1 - x^m) with m ≠ 2 (mod 3) up to the requested n
 *            and memoises the result so subsequent calls are...
 * Range:     i8 n <= 85, i16 n <= 85, i32 n <= 85, i64 n <= 85, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Memoised coefficient array */
static seq_t *cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    /* If we haven't computed enough terms yet, recompute up to n. */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        for (size_t i = 0; i < new_size; ++i)
            new_cache[i] = 0;
        new_cache[0] = 1;                /* a(0) = 1 */

        /* Multiply by (1 - x^m) for all m ≤ n with m%3 != 2. */
        for (idx_t m = 1; m <= n; ++m) {
            if ((m % 3u) == 2u)
                continue;                 /* skip exponents ≡ 2 mod 3 */
            for (idx_t k = n; k >= m; --k) {
                new_cache[k] -= new_cache[k - m];
            }
        }

        free(cache);                     /* discard old array */
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}

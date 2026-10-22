/*
 * A391972  a(n) = 1 if n=1 or a(n-1) is prime; a(n) = a(n-1) + i otherwise,
 *          where i = n - r and r is the number of occurrences of 1 among
 *          terms a(1) to a(n-1).
 * https://oeis.org/A391972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 1 or a(n-1) is prime; a(n) = a(n-1) + i
 *            otherwise, where i = n - r and r is the number of occurrences of
 *            1 among terms a(1) to a(n-1)
 * Range:     u8 n <= 41, u16 n <= 871, u32 n <= 931, u64 n <= 9398,
 *            u128 n <= 9398, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* out of range (offset = 1) */

    /* static cache */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* number of terms cached */
    static size_t ones  = 0;   /* number of 1's among cached terms */

    /* Guard: if needed, extend cache */
    if (n > cap) {
        /* allocate or reallocate */
        if (cap == 0) {
            cache = (seq_t *)malloc(sizeof(seq_t));
            if (!cache) abort();          /* allocation failed */
            cache[0] = (seq_t)1;          /* a(1) = 1 */
            cap      = 1;
            ones     = 1;
        } else {
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)n);
            if (!tmp) abort();
            cache = tmp;
        }

        /* compute missing terms */
        for (idx_t i = (idx_t)cap + 1; i <= n; ++i) {
            seq_t prev = cache[(size_t)i - 2];  /* a(i-1) */
            seq_t cur;

            if (is_prime_big(prev))          /* reset to 1 on prime */
                cur = (seq_t)1;
            else {
                /* step = i - r (r = ones count so far) */
                seq_t step = (seq_t)((idx_t)i - ones);
                cur = prev + step;
            }

            cache[(size_t)i - 1] = cur;
            if (cur == (seq_t)1) ++ones;
        }
        cap = (size_t)n;
    }

    return cache[(size_t)n - 1];
}

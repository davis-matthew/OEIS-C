/*
 * A303828  Number of ways of writing n as a sum of powers of 5, each power
 *          being used at most 6 times.
 * https://oeis.org/A303828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A303828 – number of ways of writing n as a sum of powers of 5,
 *            each power used at most 6 times. A(x) satisfies: A(x) = (1 + x +
 *            x^2 + x^3 + x^4 + x^5 + x^6) * A(x^5). The sequence is computed
 *            via the recurrence: a(0) = 1 a(n) = Σ_{i=0}^6 a((n-i)/5) for all
 *            i with n≥i and (n-i)%5==0
 * Range:     u8 n <= 9397, u16 n <= 9397, u32 n <= 9397, u64 n <= 9397,
 *            u128 n <= 9397, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache that keeps computed values up to the largest n seen. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;            /* actual allocated size (maximum n computed + 1) */

    if (n + 1 > cache_len) {
        /* Resize cache to accommodate n. */
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!tmp) { /* allocation failed – return 0 as a safe fallback */
            return (seq_t)0;
        }
        cache = tmp;
        cache_len = new_len;

        /* Base case: a(0) = 1. */
        if (old_len == 0) {
            cache[0] = (seq_t)1;
            old_len = 1;
        }

        /* Compute values for indices from old_len up to new_len-1. */
        for (idx_t k = old_len; k < new_len; ++k) {
            seq_t val = (seq_t)0;
            for (idx_t i = 0; i <= 6 && k >= i; ++i) {
                if (((k - i) % 5) == 0) {
                    val += cache[(k - i) / 5];
                }
            }
            cache[k] = val;
        }
    }
    return cache[n];
}

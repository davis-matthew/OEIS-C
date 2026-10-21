/*
 * A281006  a(n) = A000203(n) - A052928(n-1).
 * https://oeis.org/A281006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) - 2*floor((n-1)/2)
 * Range:     u8 n <= 143, u16 n <= 9706, u32 n <= 9706, u64 n <= 9706,
 *            u128 n <= 9706, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   sigma(n) is the sum of positive divisors of n.
   The function memoizes results in a static array so that
   any order of calls up to any n is handled efficiently.
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (n == 0)                /* Out of bounds – not used with OEIS offset */
        return 0;

    /* Extend cache if necessary */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache)
            return 0;          /* Allocation failed – return 0 as safe default */

        /* Initialize newly allocated entries to zero (uncomputed state) */
        if (new_len > cache_len)
            memset(new_cache + cache_len, 0,
                   (new_len - cache_len) * sizeof(seq_t));

        cache = new_cache;
        cache_len = new_len;
    }

    /* Compute only if not already cached */
    if (cache[n] == 0) {
        seq_t sigma = 0;
        idx_t i;
        for (i = 1; (idx_t)i * (idx_t)i <= n; ++i) {
            if (n % i == 0) {
                sigma += (seq_t)i;
                idx_t j = n / i;
                if (j != i)
                    sigma += (seq_t)j;
            }
        }
        seq_t sub = 2 * (seq_t)((n - 1) / 2);
        cache[n] = sigma - sub;
    }

    return cache[n];
}

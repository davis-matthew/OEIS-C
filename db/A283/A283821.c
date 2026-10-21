/*
 * A283821  a(n) = (1 + Sum_{j=1..K-2} a(n-j)*a(n-j-1))/a(n-K) with
 *          a(1),...,a(K)=1, where K=9.
 * https://oeis.org/A283821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1 + Σ_{j=1..K-2} a(n-j)*a(n-j-1)) / a(n-K) with K = 9
 *            and initial values a(1) = … = a(9) = 1
 * Range:     u8 n <= 12, u16 n <= 13, u32 n <= 15, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    idx_t i, j;

    /* initialise cache for the first K terms */
    if (cache == NULL) {
        cache_size = 9;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        /* all initial values are 1 */
        for (i = 0; i < cache_size; ++i)
            cache[i] = 1;
    }

    /* extend cache up to n if necessary */
    if (n > cache_size) {
        idx_t new_size = n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) {
            /* allocation failed; return 0 as a safe fallback */
            return 0;
        }
        cache = new_cache;
        /* compute new terms */
        for (i = cache_size + 1; i <= n; ++i) {
            seq_t value;
            if (i <= 9) {
                value = 1;
            } else {
                seq_t sum = 1;          /* initial 1 */
                /* j ranges from 1 to K-2 = 7 */
                for (j = 1; j <= 7; ++j) {
                    seq_t a1 = cache[i - j - 1];   /* a(i-j)   */
                    seq_t a2 = cache[i - j - 2];   /* a(i-j-1) */
                    sum += a1 * a2;
                }
                value = sum / cache[i - 9 - 1];   /* a(i-K) = a(i-9) */
            }
            cache[i - 1] = value;
        }
        cache_size = n;
    }

    return cache[n - 1];
}

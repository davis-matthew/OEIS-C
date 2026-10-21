/*
 * A259870  a(0)=0, a(1)=1; a(n) = n*a(n-1) + (n-2)*a(n-2) - Sum_{j=1..n-1}
 *          a(j)*a(n-j).
 * https://oeis.org/A259870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1; a(n) = n*a(n-1) + (n-2)*a(n-2) - Sum_{j=1..n-1}
 *            a(j)*a(n-j)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* memoised results */
    static size_t cache_cap = 0;     /* size of allocated cache */

    /* Ensure we have at least n+1 entries in the cache */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap == 0 ? 1 : cache_cap * 2;
        if (new_cap <= (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {                 /* allocation failure */
            exit(EXIT_FAILURE);
        }
        cache = new_cache;
        /* Initialize new entries that have not been computed yet */
        for (size_t i = cache_cap; i < new_cap; ++i) {
            if (i == 0) {
                cache[i] = 0;
            } else if (i == 1) {
                cache[i] = 1;
            } else {
                seq_t sum = 0;
                for (idx_t j = 1; j < i; ++j) {
                    sum += cache[j] * cache[i - j];
                }
                cache[i] = (seq_t)i * cache[i - 1] +
                           (seq_t)(i - 2) * cache[i - 2] -
                           sum;
            }
        }
        cache_cap = new_cap;
    }
    return cache[n];
}

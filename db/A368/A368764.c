/*
 * A368764  a(n) = n! * (1 + Sum_{k=0..n} binomial(k+3,4) / k!).
 * https://oeis.org/A368764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + binomial(n+3,4); a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   binomial, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/grow_seq.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Static cache: a[i] contains a(i) for 0 <= i < cache_len. */
    static seq_t *cache = NULL;      /* array of values */
    static size_t cache_cap = 0;     /* allocated capacity */
    static idx_t cache_len = 0;      /* number of values currently stored */

    /* Ensure the cache covers the requested index. */
    if (n >= cache_len) {
        /* Grow the array to hold indices up to n (inclusive). */
        grow_seq(&cache, &cache_cap, (size_t)(n + 1));

        /* Compute all missing terms from the last stored value up to n. */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;               /* a(0) = 1 */
            } else {
                /* Recurrence: a(i) = i * a(i-1) + C(i+3,4) */
                cache[i] = (seq_t)i * cache[i - 1] + binomial(i + 3, 4);
            }
        }
        cache_len = n + 1;      /* Update the number of stored values. */
    }

    return cache[n];
}

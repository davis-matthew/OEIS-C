/*
 * A308589  Number of minimal edge covers in the (2n-1)-triangular snake
 *          graph.
 * https://oeis.org/A308589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + a(n-3). a(1)=0, a(2)=3, a(3)=7
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_n = 0;

    if (n == 0) {
        return (seq_t)0;
    }

    if (cache_n == 0) {
        cache_n = 3;
        cache   = seq_alloc((size_t)cache_n);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)7;
    }

    if (n <= cache_n) {
        return cache[n-1];
    }

    /* extend the cached array to hold values up to n */
    if (cache_n < n) {
        cache = seq_realloc(cache, (size_t)cache_n, (size_t)n);
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_n + 1; i <= n; ++i) {
        seq_t value = 2*cache[i-2] + cache[i-3] + cache[i-4];
        cache[i-1] = value;
    }
    cache_n = n;
    return cache[n-1];
}

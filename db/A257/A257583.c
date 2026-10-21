/*
 * A257583  a(0)=4; thereafter a(n)=8*n*(2*n-1)*a(n-1).
 * https://oeis.org/A257583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 4; thereafter a(n) = 8*n*(2*n-1)*a(n-1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache  = NULL;
    static idx_t  cached = 0;   /* number of terms currently stored (size of cache) */

    /* Allocate on first call */
    if (cached == 0) {
        cache  = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failed – unlikely in the tests */
        cache[0] = (seq_t)4;            /* base case a(0) = 4 */
        cached = 1;
    }
    /* Grow the cache if the requested index is beyond current size */
    else if (n >= cached) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;    /* allocation failed */
        cache = tmp;
        /* Fill new entries */
        for (idx_t i = cached; i <= n; ++i) {
            /* a(i) = 8 * i * (2*i - 1) * a(i-1) */
            seq_t mul = (seq_t)8;
            mul = mul * (seq_t)i * ((seq_t)2 * i - (seq_t)1);
            cache[i] = cache[i - 1] * mul;
        }
        cached = n + 1;
    }
    /* Return the requested term */
    return cache[n];
}

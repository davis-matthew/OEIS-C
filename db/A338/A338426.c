/*
 * A338426  a(n) is the number of paths a chess king can take from (0,0) to
 *          (n+1,0) touching each point in {-1,0,1} X {1,2,...,n} exactly
 *          once.
 * https://oeis.org/A338426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 7*a(n-1) + 6*a(n-2) - 39*a(n-3) + 29*a(n-4) + 28*a(n-5) - 26*a(n-6) - 10*a(n-7) + 6*a(n-8)
   with a(0)=1, a(1)=2, a(2)=28, a(3)=154, a(4)=1206, a(5)=8364, a(6)=60614, a(7)=432636 */

#include <stddef.h>
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static const seq_t init[8] = {
        (seq_t)1,  (seq_t)2,  (seq_t)28,  (seq_t)154,
        (seq_t)1206,(seq_t)8364,(seq_t)60614,(seq_t)432636
    };

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n < 8)
        return init[n];

    if (n >= cache_len) {
        size_t newlen = (size_t)n + 1;
        cache = seq_realloc(cache, cache_len, newlen);
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 8)
                cache[i] = init[i];
            else
                cache[i] = (seq_t)7  * cache[i-1]
                         + (seq_t)6  * cache[i-2]
                         + (seq_t)-39 * cache[i-3]
                         + (seq_t)29 * cache[i-4]
                         + (seq_t)28 * cache[i-5]
                         + (seq_t)-26 * cache[i-6]
                         + (seq_t)-10 * cache[i-7]
                         + (seq_t)6  * cache[i-8];
        }
        cache_len = newlen;
    }

    return cache[n];
}

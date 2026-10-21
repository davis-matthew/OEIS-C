/*
 * A261442  Number of binary strings of length n+6 such that the smallest
 *          number whose binary representation is not visible in the string is
 *          6.
 * https://oeis.org/A261442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 179,
 *            big any n
 * Helpers:   alloc_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A261442
   Recurrence: a(0)=0, a(1)=2, a(2)=6, a(3)=15, a(4)=32, a(5)=64, a(6)=120,
               a(7)=218, a(8)=385.
   For n ≥ 9:
       a(n) = 4a(n-1) - 4a(n-2) - 2a(n-3) + 4a(n-4)
              - a(n-5) + 2a(n-6) - 2a(n-7) - a(n-8) + a(n-9).
*/
#include <stdlib.h>
#include <stddef.h>
#include "../utils/alloc_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initialise cache with first nine terms */
    if (cache_len == 0) {
        cache_len = 9;
        cache = (seq_t *)alloc_cache(cache_len);   /* alloc_cache returns seq_t * */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)15;
        cache[4] = (seq_t)32;
        cache[5] = (seq_t)64;
        cache[6] = (seq_t)120;
        cache[7] = (seq_t)218;
        cache[8] = (seq_t)385;
    }

    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* grow cache to hold a[n] */
    size_t needed = (size_t)n + 1;
    if (needed > cache_len) {
        cache_len = needed;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_len);
    }

    /* compute new values using the recurrence */
    for (size_t i = 9; i <= (size_t)n; ++i) {
        seq_t val =  (seq_t)4 * cache[i-1]
                    - (seq_t)4 * cache[i-2]
                    - (seq_t)2 * cache[i-3]
                    + (seq_t)4 * cache[i-4]
                    - cache[i-5]
                    + (seq_t)2 * cache[i-6]
                    - (seq_t)2 * cache[i-7]
                    - cache[i-8]
                    + cache[i-9];
        cache[i] = val;
    }

    return cache[(size_t)n];
}

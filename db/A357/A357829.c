/*
 * A357829  a(n) = Sum_{k=0..floor((n-1)/3)} |Stirling1(n,3*k+1)|.
 * https://oeis.org/A357829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/3)} |Stirling1(n,3*k+1)|
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static seq_t R0 = 0, R1 = 0, R2 = 0;
    static bool init = false;

    if (!init) {
        /* initialise for n = 0 */
        cache      = alloc_seq_array(1);
        cache[0]   = 0;        /* a(0) = 0 */
        cache_size = 1;
        R0 = (seq_t)1;
        R1 = (seq_t)0;
        R2 = (seq_t)0;
        init = true;
    }

    if (n < cache_size)
        return cache[n];

    for (idx_t cur = cache_size; cur <= n; ++cur) {
        seq_t factor = (seq_t)(cur - 1);

        seq_t newR0 = R2 + factor * R0;
        seq_t newR1 = R0 + factor * R1;
        seq_t newR2 = R1 + factor * R2;

        R0 = newR0;
        R1 = newR1;
        R2 = newR2;

        /* extend cache by one element */
        idx_t newsize   = cache_size + 1;
        seq_t *newcache = alloc_seq_array(newsize);

        for (idx_t i = 0; i < cache_size; ++i)
            newcache[i] = cache[i];

        newcache[cache_size] = R1;   /* a(cur) */

        free_seq_array(cache);
        cache      = newcache;
        cache_size = newsize;
    }

    return cache[n];
}

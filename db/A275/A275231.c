/*
 * A275231  Number of 6 X n 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-2,1) or (-1,0) and new values introduced in order
 *          0..2.
 * https://oeis.org/A275231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 19*a(n-1) - 75*a(n-2) + 101*a(n-3) - 44*a(n-4) for n>5,
 *            a(1)=16, a(2)=162, a(3)=2282, a(4)=32388, a(5)=460034
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* Cache of already computed values.
       Indices 1..cache_len are valid; cache[0] is unused. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;      /* highest index computed so far */

    if (n == 0)                     /* undefined, return 0 */
        return (seq_t)0;

    /* Ensure we have storage large enough for n. */
    if (n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 64;
        while (new_cap < (size_t)n)
            new_cap <<= 1;          /* double until big enough */

        seq_t *new_cache = seq_alloc_new_array(new_cap);
        if (!new_cache)
            return (seq_t)0;        /* allocation failure */

        /* Copy old values */
        for (size_t i = 1; i <= cache_len; ++i)
            new_cache[i] = cache[i];

        if (cache)
            seq_free_array(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialise the first five terms if not already done. */
    if (cache_len == 0) {
        cache[1] = (seq_t)16;
        cache[2] = (seq_t)162;
        cache[3] = (seq_t)2282;
        cache[4] = (seq_t)32388;
        cache[5] = (seq_t)460034;
        cache_len = 5;
    }

    /* If the requested term is already in the cache, return it. */
    if (n <= cache_len)
        return cache[n];

    /* Compute new terms up to n using the recurrence. */
    for (idx_t k = (idx_t)(cache_len + 1); k <= n; ++k) {
        seq_t t1 = 19 * cache[k - 1];
        t1 += 101 * cache[k - 3];

        seq_t t2 = 75 * cache[k - 2];
        t2 += 44 * cache[k - 4];

        cache[k] = t1 - t2;          /* t1 >= t2 for all n ≥ 6 */
    }

    cache_len = (size_t)n;
    return cache[n];
}

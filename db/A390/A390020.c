/*
 * A390020  a(n) = Sum_{k=0..floor(2*n/5)} binomial(k+1,2*n-5*k).
 * https://oeis.org/A390020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-3) + a(n-5) - a(n-6) for n≥6 Initial values:
 *            a(0)=1, a(1)=0, a(2)=0, a(3)=2, a(4)=0, a(5)=1 The
 *            implementation memoises previously computed terms so that any
 *            sequence of calls returns the correct value in O(n) overall
 * Range:     u8 n <= 26, u16 n <= 48, u32 n <= 91, u64 n <= 179,
 *            u128 n <= 355, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* number of values stored */
    static idx_t cache_cap  = 0; /* allocated capacity */

    /* Initialise with the first six values if needed */
    if (cache_size == 0) {
        cache_cap = 6;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* memory failure – unlikely */
        cache[0] = 1;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 2;
        cache[4] = 0;
        cache[5] = 1;
        cache_size = 6;
    }

    /* Return cached value if already computed */
    if (n < cache_size)
        return cache[n];

    /* Grow the cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i >= 6) {
            seq_t val = 2 * cache[i - 3] + cache[i - 5];
            val -= cache[i - 6];
            cache[i] = val;
        }
    }
    cache_size = n + 1;
    return cache[n];
}

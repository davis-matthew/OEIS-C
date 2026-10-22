/*
 * A313524  Coordination sequence Gal.5.219.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 2*a(n-1) - 3*a(n-2) + 4*a(n-3) - 5*a(n-4) + 6*a(n-5) *
 *            - 5*a(n-6) + 4*a(n-7) - 3*a(n-8) + 2*a(n-9) - a(n-10) * for n >
 *            10, with initial values: *
 *            a(0)=1,a(1)=5,a(2)=10,a(3)=14,a(4)=18,a(5)=24, *
 *            a(6)=30,a(7)=34,a(8)=38,a(9)=42,a(10)=46
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t len   = 0;   /* number of values stored in cache (cache[0..len-1]) */
    static size_t cap  = 0;   /* allocated capacity */

    /* Return cached value if already computed */
    if (n < len)
        return cache[n];

    /* Increase cache capacity if necessary */
    if (cache == NULL) {
        /* Allocate initial space large enough for the first call */
        cap = (size_t)(n + 1) > 16 ? (size_t)(n + 1) : 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure: return 0 */
    } else if ((idx_t)cap <= n) {
        size_t newcap = cap;
        while ((idx_t)newcap <= n)
            newcap <<= 1;          /* double until we have enough */
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cap = newcap;
    }

    /* Initial values for indices 0..10 */
    static const seq_t init[11] = {
        (seq_t)1,  (seq_t)5,  (seq_t)10, (seq_t)14, (seq_t)18,
        (seq_t)24, (seq_t)30, (seq_t)34, (seq_t)38, (seq_t)42,
        (seq_t)46
    };

    /* Compute missing values up to n */
    for (idx_t i = len; i <= n; ++i) {
        if (i <= 10) {
            cache[i] = init[i];
        } else {
            /* Use a signed 64‑bit integer for intermediate calculations
               to avoid unsigned underflow during the signed additions/subtractions. */
            long long v = 0;
            v += (long long)cache[i-1] * 2;
            v -= (long long)cache[i-2] * 3;
            v += (long long)cache[i-3] * 4;
            v -= (long long)cache[i-4] * 5;
            v += (long long)cache[i-5] * 6;
            v -= (long long)cache[i-6] * 5;
            v += (long long)cache[i-7] * 4;
            v -= (long long)cache[i-8] * 3;
            v += (long long)cache[i-9] * 2;
            v -= (long long)cache[i-10];
            cache[i] = (seq_t)v;
        }
    }
    len = n + 1;               /* update cache length */
    return cache[n];
}

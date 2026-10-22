/*
 * A387689  a(n) = Sum_{k=0..floor(n/2)} 2^(n-2*k) * binomial(2*n-2*k,2*k).
 * https://oeis.org/A387689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} 2^(n-2k) * binomial(2n-2k, 2k) Recurrence:
 *            a(0)=1, a(1)=2, a(2)=5, a(3)=20, a(n) = 4*a(n-1) - 2*a(n-2) +
 *            4*a(n-3) - a(n-4) (n≥4)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

/* Cache for previously computed terms.
   cache[n] holds a(n) for 0 ≤ n ≤ cache_nmax. */
static seq_t *   cache   = NULL;
static size_t   cache_cap = 0;
static idx_t    cache_nmax = 0;

seq_t a(idx_t n)
{
    /* Direct answers for the first four terms. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)20;

    /* Ensure array large enough.  ensure_cache(n, &cache, &cache_cap)
       guarantees that cache has room for indices 0..n inclusive. */
    if (cache == NULL) {
        /* Allocate space for the first four values. */
        ensure_cache(3, &cache, &cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)20;
        cache_nmax = 3;
    } else if (n > cache_nmax) {
        /* Grow cache if necessary. */
        ensure_cache(n, &cache, &cache_cap);
    }

    /* If we already have the requested value, just return it. */
    if (n <= cache_nmax) {
        return cache[n];
    }

    /* Compute successive terms using the recurrence until n is reached. */
    for (idx_t i = cache_nmax + 1; i <= n; ++i) {
        /* t1 = 4*a(i-1),   t2 = 2*a(i-2),   t3 = 4*a(i-3)  */
        seq_t t1 = cache[i-1];  t1 <<= 2;          /* *4 */
        seq_t t2 = cache[i-2];  t2 <<= 1;          /* *2 */
        seq_t t3 = cache[i-3];  t3 <<= 2;          /* *4 */

        /* a(i) = t1 - t2 + t3 - a(i-4) */
        seq_t val = t1 + t3;
        val -= t2;
        val -= cache[i-4];
        cache[i] = val;
    }

    /* Update the maximum computed index. */
    cache_nmax = n;
    return cache[n];
}

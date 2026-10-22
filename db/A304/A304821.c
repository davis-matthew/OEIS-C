/*
 * A304821  For n > 3, a(n) = a(n-3) + lcm(a(n-3), n-2) with a(1)=6, a(2)=6,
 *          a(3)=6.
 * https://oeis.org/A304821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-3) + lcm(a(n-3), n-2), a(1)=6, a(2)=6, a(3)=6 LCM is
 *            computed as (a/gcd) * (n-2) to avoid overflow
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 30, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* static cache: cache[1] .. cache[computed] are valid */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;          /* allocated capacity (terms + 1) */
    static idx_t  computed = 0;          /* highest n computed so far */

    /* Base cases: return 6 for n <= 3 without allocating cache */
    if (n <= 3)
        return (seq_t)6;

    /* Allocate cache on first call */
    if (cache == NULL) {
        cap = (size_t)n + 1;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[1] = cache[2] = cache[3] = (seq_t)6;
        computed = 3;
    }
    /* Grow the cache if needed */
    else if ((size_t)n + 1 > cap) {
        cap = (size_t)n + 1;
        cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));
    }

    /* Compute missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t prev = cache[i - 3];
        seq_t denom = (seq_t)(i - 2);
        seq_t g = gcd(prev, denom);          /* gcd of previous term and i-2 */
        seq_t lcm = (prev / g) * denom;      /* lcm = prev/g * (i-2) */
        cache[i] = prev + lcm;
    }
    computed = n;

    return cache[n];
}

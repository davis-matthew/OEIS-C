/*
 * A249638  Number of strings of length n over a 5-letter alphabet that begin
 *          with a nontrivial palindrome.
 * https://oeis.org/A249638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, walk
 * Formula:   a(n) = 5*a(n-1)+5^ceil(n/2)-a(ceil(n/2)), with a(0)=0, a(1)=0.
 *            The recurrence holds for n≥2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n <= 1)
        return (seq_t)0;

    /* Static cache to avoid repeated allocations. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    ensure_cache(n + 1, &cache, &cap);   /* guaranteed size ≥ n+1 */

    cache[0] = (seq_t)0;
    cache[1] = (seq_t)0;

    /* pow5 holds 5^ceil(k/2) for the current k.  For k=2, ceil=1. */
    seq_t pow5 = (seq_t)5;

    for (idx_t k = 2; k <= n; ++k)
    {
        /* When k is odd, ceil(k/2) increases by 1 → multiply by 5. */
        if ((k & 1) == 1)
            pow5 *= (seq_t)5;

        idx_t idx      = (k + 1) >> 1;        /* ceil(k/2) */
        cache[k] = (seq_t)5 * cache[k - 1] + pow5 - cache[idx];
    }

    return cache[n];
}

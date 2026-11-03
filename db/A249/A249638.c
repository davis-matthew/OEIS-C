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
 * Validated: standard, pass, 40 terms through n = 986 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

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

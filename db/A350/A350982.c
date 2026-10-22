/*
 * A350982  a(0)=0, a(1)=49, thereafter a(n) = 14*a(n-1)-a(n-2)+42.
 * https://oeis.org/A350982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) - a(n-2) + 42, a(0)=0, a(1)=49
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_cached = (idx_t)-1; /* indicates uninitialized */

    /* initialize cache for the first two terms if not done yet */
    if (max_cached == (idx_t)-1) {
        ensure_capacity(2, &cap, (void **)&cache, sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)49;
        max_cached = 1;
    }

    /* trivial cases */
    if (n == 0) return cache[0];
    if (n == 1) return cache[1];

    /* compute missing terms up to n */
    if (n > max_cached) {
        ensure_capacity((size_t)n + 1, &cap, (void **)&cache, sizeof(seq_t));
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            cache[i] = (seq_t)14 * cache[i - 1] - cache[i - 2] + (seq_t)42;
        }
        max_cached = n;
    }

    return cache[n];
}

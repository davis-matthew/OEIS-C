/*
 * A284054  Relative of Hofstadter Q-sequence.
 * https://oeis.org/A284054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n <= 0 a(1) = 7, a(2) = 26, a(3) = 8, a(4) = 8,
 *            a(5) = 8, a(6) = 8, a(7) = 8, a(8) = 26, a(9) = 7, a(10)= 8,
 *            a(11)=16, a(12)=16, a(13)=16, a(14)=16, a(15)=16, a(16)=26 For n
 *            > 16 : a(n) = a(n - a(n-1)) + a(n - a(n-2))
 * Range:     u8 n <= 111, u16 n <= 343, u32 n <= 799, u64 n <= 1703,
 *            u128 n <= 3519, big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/grow_seq_array.h"

static seq_t *cache = NULL;           /* memoization table */
static idx_t cache_max = 0;           /* largest index computed (inclusive) */
static idx_t cache_cap = 0;           /* current allocated capacity */

/* Ensure that cache[0..max(n,cache_max)] is computed. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max)
        return;

    /* Resize cache to hold indices 0..n (n+1 elements). */
    if (cache_cap < n + 1)
        grow_seq_array(&cache, &cache_cap, n + 1);

    /* Base values for indices 0..16 (index 0 is 0 for n <= 0). */
    static const seq_t base_vals[17] = {
        0, 7, 26, 8, 8, 8, 8, 8, 26, 7, 8,
        16, 16, 16, 16, 16, 26
    };

    /* Set cache[0] (used when n - a(n-1) or n - a(n-2) <= 0). */
    if (cache_cap > 0)
        cache[0] = base_vals[0];

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i <= 16) {
            cache[i] = base_vals[i];
        } else {
            /* Compute the two recursive indices, clamping to 0 if non‑positive. */
            idx_t idx1 = (i > (idx_t)cache[i - 1]) ? i - (idx_t)cache[i - 1] : 0;
            idx_t idx2 = (i > (idx_t)cache[i - 2]) ? i - (idx_t)cache[i - 2] : 0;
            cache[i] = cache[idx1] + cache[idx2];
        }
    }
    cache_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* defined for n <= 0 */
    ensure_cache(n);
    return cache[n];
}

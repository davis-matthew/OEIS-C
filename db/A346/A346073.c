/*
 * A346073  a(n) = 1 + Sum_{k=0..n-4} a(k) * a(n-k-4).
 * https://oeis.org/A346073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{k=0}^{n-4} a(k)*a(n-4-k), initial values:
 *            a(0)=a(1)=a(2)=a(3)=1
 * Range:     u8 n <= 15, u16 n <= 26, u32 n <= 48, u64 n <= 89, u128 n <= 170,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;       /* Array for memoized values */
static size_t cap = 0;            /* Allocated capacity */
static idx_t last_filled = 0;     /* Next index to compute */

/* Compute or retrieve the sequence value a(n) */
seq_t a(idx_t n)
{
    if (n < 4) {
        /* Base values */
        return (seq_t)1;
    }

    /* Ensure space for indices up to n and initialize base values if needed */
    if (last_filled == 0) {
        ensure_cache(3, &cache, &cap);          /* Need at least indices 0..3 */
        cache[0] = cache[1] = cache[2] = cache[3] = (seq_t)1;
        last_filled = 4;
    }

    /* Grow the cache array if it doesn't cover n */
    ensure_cache(n, &cache, &cap);

    /* Compute any missing values up to n */
    for (idx_t i = last_filled; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t limit = i - 4;
        for (idx_t k = 0; k <= limit; ++k) {
            sum += cache[k] * cache[limit - k];
        }
        cache[i] = (seq_t)1 + sum;
    }

    last_filled = n + 1;   /* All values up to n are now known */

    return cache[n];
}

/*
 * A386203  a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/4)} binomial(n-1,4*k) *
 *          a(4*k) * a(n-1-4*k).
 * https://oeis.org/A386203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/4)} binomial(n-1,4*k) *
 *            a(4*k) * a(n-1-4*k)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence holds for n >= 0. The function memoises results internally. */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binom.h"   /* For seq_t binom(idx_t a, idx_t b); */

static seq_t *cache = NULL;      /* Memoised values */
static size_t cache_cap = 0;     /* Allocated capacity (size_t) */
static idx_t computed = 0;       /* First index that is not yet computed */

/* Grow the cache so that indices 0..n are valid (inclusive). */
static void ensure_cache_size(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (needed > new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && needed > 0)
            abort();                    /* Allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Compute a(n) using the recurrence, memoising intermediate results. */
seq_t a(idx_t n)
{
    ensure_cache_size(n);

    /* Initialise base case if not yet done */
    if (computed == 0) {
        cache[0] = 1;
        computed = 1;
    }

    /* Compute all missing values up to n (in order to preserve memoisation). */
    if (n >= computed) {
        for (idx_t m = computed; m <= n; ++m) {
            seq_t sum = 0;
            idx_t limit = (m == 0) ? 0 : (m - 1) / 4;
            for (idx_t k = 0; k <= limit; ++k) {
                idx_t r = 4 * k;
                idx_t rest = m - 1 - r;          /* m-1 >= r by construction */
                seq_t bin = binom(m - 1, r);     /* binomial coefficient */
                sum += bin * cache[r] * cache[rest];
            }
            cache[m] = sum;
        }
        computed = n + 1;
    }

    return cache[n];
}

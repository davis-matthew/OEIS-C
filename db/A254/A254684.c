/*
 * A254684  Fifth partial sums of seventh powers (A001015).
 * https://oeis.org/A254684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(1 + n)*(2 + n)*(3 + n)*(4 + n)*(5 + n)*(-3 + 5*n +
 *            n^2)*(-2 + 5*n + n^2)*(5 + 5*n + n^2)/95040 Recurrence: a(n) =
 *            5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) + n^7
 *            Initial terms: a(1)=1, a(2)=133, a(3)=2842, a(4)=29274,
 *            a(5)=197400
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 14, u64 n <= 102, u128 n <= 1000,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/seq_pow.h"

/* Memoization cache */
static seq_t *cache = NULL;
static size_t cache_len = 0; /* current number of stored terms */
static size_t cache_cap = 0; /* current allocated capacity */

static const seq_t init[5] = {1, 133, 2842, 29274, 197400};

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1, but guard against zero */

    /* Ensure the cache large enough */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 4;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort(); /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* If already computed up to n, just return the cached value */
    if ((size_t)n <= cache_len) {
        return cache[(size_t)n - 1];
    }

    /* Compute terms beyond the current cache up to n */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        seq_t val;
        size_t idx = i + 1; /* current n value (1‑based) */

        if (i < 5) {
            val = init[i];
        } else {
            /* n^7 part */
            seq_t n_pow7 = seq_pow((seq_t)idx, 7);

            /* Positive and negative parts of the recurrence to avoid underflow */
            seq_t pos = 5 * cache[i - 1] + 10 * cache[i - 3] + cache[i - 5];
            seq_t neg = 10 * cache[i - 2] + 5 * cache[i - 4];

            val = pos - neg + n_pow7;
        }

        cache[i] = val;
    }

    cache_len = (size_t)n;
    return cache[(size_t)n - 1];
}

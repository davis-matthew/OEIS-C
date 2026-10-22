/*
 * A390222  a(n) = Sum_{k=0..floor(n/2)} binomial(k+2,4*n-8*k+2).
 * https://oeis.org/A390222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(k+2, 4*n-8*k+2)
 * Range:     u8 n <= 18, u16 n <= 35, u32 n <= 69, u64 n <= 137,
 *            u128 n <= 273, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-2) - 6*a(n-4) + 4*a(n-6) - a(n-8) + a(n-9) */
/* Initial values (n=0..8) are computed directly by the summation. */

#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;
static idx_t  cache_size = 0;
static idx_t  cache_cap  = 0;

seq_t a(idx_t n)
{
    if (n < cache_size)
        return cache[n];

    /* Grow cache if necessary. */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();           /* Allocation failed. */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing entries. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val;
        if (i <= 8) {                     /* Base: compute directly. */
            seq_t sum = 0;
            idx_t maxk = i / 2;
            for (idx_t k = 0; k <= maxk; ++k)
                sum += binomial(k + 2, 4 * i - 8 * k + 2);
            val = sum;
        } else {                          /* Recurrence. */
            val = 4 * cache[i - 2];
            val -= 6 * cache[i - 4];
            val += 4 * cache[i - 6];
            val -= cache[i - 8];
            val += cache[i - 9];
        }
        cache[i] = val;
    }

    cache_size = n + 1;
    return cache[n];
}

/*
 * A393272  a(0) = 0, a(n) = Sum_{k=1..n} binomial(n+1, k+1)*A000169(k) for n
 *          > 0.
 * https://oeis.org/A393272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} binomial(n+1, k+1)*k^(k-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   ensure_cache, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The values of A000169(k) used here are k^(k-1) */

#include <stddef.h>
#include "../utils/pow_int.h"
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* Make sure we have room for the requested index */
    if (n + 1 > cache_cap)
        ensure_cache(n + 1, &cache, &cache_cap);

    /* Initialise first values if this is the first call */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        if (n >= 1)
            cache[1] = (seq_t)1;
        cache_len = (n >= 1) ? 2 : 1;
    }

    /* Compute new terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)0;
            continue;
        }
        if (i == 1) {
            cache[1] = (seq_t)1;
            continue;
        }

        seq_t sum = (seq_t)0;

        /* binomial(i+1, 2) = (i+1)*i/2 */
        seq_t binom = ((seq_t)(i + 1) * (seq_t)i) / 2;

        for (idx_t k = 1; k <= i; ++k) {
            seq_t pow = pow_int((seq_t)k, k - 1);
            sum += binom * pow;

            /* Prepare binomial(i+1, k+2) from binomial(i+1, k+1) */
            if (k < i) /* only update if we have a next k */
            {
                seq_t num = (seq_t)(i - k);     /* = (i+1) - (k+1) */
                seq_t den = (seq_t)(k + 2);     /* = (k+1) + 1 */
                binom = (binom * num) / den;   /* exact division */
            }
        }
        cache[i] = sum;
    }

    cache_len = n + 1;
    return cache[n];
}

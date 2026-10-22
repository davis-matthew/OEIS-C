/*
 * A364735  G.f. satisfies A(x) = 1 + x*A(x) / (1 + x*A(x)^2).
 * https://oeis.org/A364735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/n) * Σ_{k=0}^{n-1} (-1)^k * binomial(n,k) *
 *            binomial(n+k, n-1-k), with a(0) = 1. Uses memoisation for
 *            arbitrary access order
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 22, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k); */

/* static cache for computed terms */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* compute a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    /* extend cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure is silent */
        cache = tmp;

        /* compute missing terms from current size to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;          /* a(0) = 1 */
                continue;
            }

            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k < i; ++k) {
                seq_t b1 = binomial(i, k);
                seq_t b2 = binomial(i + k, i - 1 - k);
                /* use 128‑bit product to avoid signed overflow */
                __int128 prod128 = (__int128)b1 * (__int128)b2;
                seq_t prod = (seq_t)prod128;

                if (k & 1)
                    sum -= prod;               /* odd k → negative */
                else
                    sum += prod;               /* even k → positive */
            }
            cache[i] = sum / (seq_t)i;          /* integer division by n */
        }

        cache_size = new_size;
    }

    return cache[n];
}

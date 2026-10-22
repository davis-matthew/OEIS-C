/*
 * A372239  Expansion of (1 + 2*x) / ((1 - 2*x)*sqrt(1 - 4*x)).
 * https://oeis.org/A372239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>=1, a(n) = 2*a(n-1) + 2*binomial(2*n-2,
 *            n-1)*(3*n-1)/n (OEIS A372239)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Use utilities from the build. */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached = 0;           /* number of elements currently cached (>=1 if cache[0] is set) */

    /* Ensure we have storage for values up to n. */
    if (n >= cached) {
        idx_t old_cached = cached;
        idx_t needed = n + 1;          /* indices 0..n */

        if (cache == NULL) {
            cache = (seq_t*)malloc(needed * sizeof(seq_t));
            if (!cache) return 0;     /* unlikely but safe */
        } else if (needed > cached) {
            seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
            if (!tmp) return cache[0];
            cache = tmp;
        }

        if (old_cached == 0) {        /* initialize a(0) */
            cache[0] = 1;
            old_cached = 1;
        }

        for (idx_t i = old_cached; i <= n; ++i) {   /* compute missing terms */
            seq_t bin = binomial(2*i-2, i-1);        /* C(2*i-2, i-1) */
            seq_t tmp = bin / i;                    /* bin / i  (integral) */
            seq_t term = tmp * 2;                   /* 2*tmp */
            term *= (seq_t)(3*i - 1);               /* * (3*i-1) */

            cache[i] = cache[i-1] * 2;              /* 2*a(i-1) */
            cache[i] += term;                      /* + term */
        }

        cached = needed;
    }

    return cache[n];
}

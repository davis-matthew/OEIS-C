/*
 * A381276  Expansion of e.g.f. exp(x * cos(3*x)).
 * https://oeis.org/A381276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=0..⌊(n-1)/2⌋} (-9)^k * (2*k+1) * binomial(n-1,2*k) *
 *            a(n-2*k-1), a(0)=1
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 11, i64 n <= 18, i128 n <= 32,
 *            big any n
 * Helpers:   binom, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A381276 – Expansion of e.g.f. exp(x * cos(3*x)).
   The implementation caches results so that calls in any order are fast. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/ensure_cache.h"
#include "../utils/binom.h"

/* seq_t and idx_t are supplied by the build configuration. */
seq_t a(idx_t n)
{
    /* static cache of already computed terms a(0)..a(N) */
    static seq_t *cache   = NULL;   /* array holding the terms   */
    static size_t  cap    = 0;      /* allocated capacity        */
    static idx_t   last   = 0;      /* highest index already computed */

    /* enlarge the cache if necessary */
    ensure_cache(n, &cache, &cap);

    /* initialise a(0) if it hasn't been done yet      */
    if (last == 0 && cache[0] == 0) {
        cache[0] = (seq_t)1;
        last = 0;
    }

    /* already computed?  Just return it. */
    if (n <= last)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t maxk = (i > 0) ? ((i - 1) >> 1) : 0;   /* floor((i-1)/2) */

        seq_t pow_neg9 = (seq_t)1;   /* (-9)^k, start with k=0 */

        for (idx_t k = 0; k <= maxk; ++k) {
            if (k > 0)
                pow_neg9 = pow_neg9 * (seq_t)(-9);     /* multiply by -9 each step */

            seq_t coeff = pow_neg9 * (seq_t)(2 * k + 1);  /* (2*k+1)*(-9)^k */
            seq_t bin   = binom(i - 1, 2 * k);           /* binomial(i-1,2k) */
            seq_t term  = coeff * bin * cache[i - 2 * k - 1];

            sum += term;
        }

        cache[i] = sum;
    }

    last = n;
    return cache[n];
}

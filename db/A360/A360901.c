/*
 * A360901  G.f. satisfies A(x) = 1 + x/(1 + x^4)^2 * A(x/(1 + x^4)).
 * https://oeis.org/A360901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = Σ_{k=0..⌊(n-1)/4⌋} (-1)^k * binomial(n-3k, k) *
 *            a(n-1-4k)
 * Range:     i8 n <= 12, i16 n <= 21, i32 n <= 33, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   binomial, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static idx_t  computed_upto = 0;

    /* initialise cache for n = 0 if necessary */
    if (n == 0) {
        if (cache == NULL) {
            ensure_cache(1, &cache, &cap);
            cache[0] = 1;
            computed_upto = 0;
        }
        return cache[0];
    }

    /* ensure we have storage up to n */
    if (cache == NULL) {
        ensure_cache(1, &cache, &cap);
        cache[0] = 1;
        computed_upto = 0;
    }
    if (n >= (idx_t)cap) {
        /* simply request a bigger cache from the helper */
        ensure_cache(n + 1, &cache, &cap);
    }

    /* compute missing terms up to n */
    if (n > computed_upto) {
        for (idx_t m = computed_upto + 1; m <= n; ++m) {
            idx_t kmax   = (m - 1) / 4;
            seq_t sum    = 0;
            for (idx_t k = 0; k <= kmax; ++k) {
                seq_t bin   = binomial(m - 3 * k, k);
                seq_t sub   = cache[m - 1 - 4 * k];
                seq_t term  = bin * sub;
                if (k & 1) {
                    term = -term;
                }
                sum += term;
            }
            cache[m] = sum;
        }
        computed_upto = n;
    }

    return cache[n];
}

/*
 * A368287  Expansion of e.g.f. exp(-2*x) / (1 + 2*log(1 - x)).
 * https://oeis.org/A368287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-2)^n + 2 * Sum_{k=1..n} (k-1)! * binomial(n,k) * a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   binomial, ensure_cache, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial value: a(0) = 1 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ensure_cache.h"
#include "../utils/binomial.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* static memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t computed = 0;          /* highest index computed so far */
    static seq_t pow2 = 1;              /* stores 2^i for the current i */

    /* make sure cache is large enough */
    ensure_cache(n + 1, &cache, &cache_cap);

    /* initialise for the very first call */
    if (computed == 0) {
        cache[0] = (seq_t)1;
        /* pow2 already == 1, computed == 0 */
    }

    /* compute from last computed + 1 up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* update pow2 to 2^i */
        if (i > 0) {
            pow2 = pow2 * (seq_t)2;          /* 2^i */
        }

        /* accumulate sum of (k-1)! * binomial(i,k) * a(i-k) */
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t term = binomial(i, k);          /* binomial(i,k)   */
            term = term * factorial(k - 1);        /* * (k-1)!       */
            term = term * cache[i - k];            /* * a(i-k)       */
            sum += term;                           /* accumulate     */
        }

        sum = sum * ((seq_t)2); /* multiply sum by 2 */

        /* compute (-2)^i, using two's complement arithmetic */
        seq_t two_n = pow2;
        if (i & 1) {
            two_n = -two_n; /* odd n: negate 2^i */
        }

        /* final value for a(i) */
        cache[i] = sum + two_n;
    }

    computed = n;
    return cache[n];
}

/*
 * A330149  Expansion of e.g.f. exp(-x) / (1 + log(1 - x)).
 * https://oeis.org/A330149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n + Sum_{k=1..n} (k-1)! * binomial(n,k) * a(n-k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n) {
    /* Cached sequences */
    static seq_t *a_cache   = NULL;
    static size_t a_cap    = 0;
    static size_t a_max    = 0;        /* largest n already computed */

    /* Cached factorials */
    static seq_t *fac       = NULL;
    static size_t fac_cap   = 0;
    static size_t fac_max   = 0;        /* largest k with fac[k] computed */

    /* Make sure there is room for the requested n */
    ensure_cache(n, &a_cache, &a_cap);
    ensure_cache(n, &fac,    &fac_cap);

    /* Initialize base cases if this is the first use */
    if (fac_max == 0) {
        fac[0] = 1;
        fac_max = 0;
    }
    if (a_max == 0) {
        a_cache[0] = 1;
    }

    /* Compute factorials up to n */
    for (idx_t k = fac_max + 1; k <= n; ++k) {
        fac[k] = fac[k - 1] * (seq_t)k;
    }
    fac_max = n;

    /* Compute a(n) for all values from a_max+1 up to n */
    for (idx_t i = a_max + 1; i <= n; ++i) {
        seq_t sum   = 0;
        seq_t binom = 1;                 /* binomial(i,0) */

        for (idx_t k = 1; k <= i; ++k) {
            /* Update binomial coefficient binom = C(i, k) */
            binom = binom * (seq_t)(i - k + 1);
            binom = binom / (seq_t)k;    /* exact division */

            /* term = (k-1)! * binom * a(i-k) */
            seq_t term = fac[k - 1];
            term = term * binom;
            term = term * a_cache[i - k];

            sum += term;
        }

        /* Add the (-1)^i part */
        if ((i & 1) == 0) {     /* i even -> (+1) */
            sum += (seq_t)1;
        } else {                /* i odd  -> (-1) */
            sum -= (seq_t)1;
        }

        a_cache[i] = sum;
    }
    a_max = n;

    return a_cache[n];
}

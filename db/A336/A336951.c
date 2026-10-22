/*
 * A336951  E.g.f.: 1 / (1 - x * exp(3*x)).
 * https://oeis.org/A336951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A336951
   a(0) = 1
   a(n) = Σ_{k=1..n} binomial(n,k) * k * 3^(k-1) * a(n-k)   (n ≥ 1)
   Implementation uses memoisation and a simple O(n²) recurrence. */

#include <stddef.h>          /* size_t */
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t computed = 0;   /* number of pre‑computed entries [0, computed-1] */

    /* initialise first term */
    if (computed == 0) {
        if (cache_cap < 1)
            grow_seq(&cache, &cache_cap, 1);
        cache[0] = 1;
        computed = 1;
    }

    /* if requested value already known, return it */
    if (n < computed)
        return cache[n];

    /* ensure capacity for all needed values */
    if (cache_cap < (size_t)(n + 1))
        grow_seq(&cache, &cache_cap, (size_t)n + 1);

    /* compute missing values from computed up to n */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t sum  = 0;
        seq_t binom = i;           /* C(i,1) */
        seq_t power3 = 1;          /* 3^(k-1) for current k */

        for (idx_t k = 1; k <= i; ++k) {
            /* term = binom * k * power3 * a(i-k) */
            seq_t term = binom;
            term *= (seq_t)k;
            term *= power3;
            term *= cache[i - k];
            sum += term;

            if (k == i)   /* last iteration, no next binomial/power needed */
                break;

            /* update binomial: C(i,k+1) = C(i,k) * (i-k)/(k+1) */
            binom = binom * ((seq_t)(i - k)) / (k + 1);
            /* update power of 3: 3^k = 3^(k-1) * 3 */
            power3 *= 3;
        }
        cache[i] = sum;
    }
    computed = n + 1;
    return cache[n];
}

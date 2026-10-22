/*
 * A367940  Expansion of e.g.f. exp(exp(4*x) - 1 - 3*x).
 * https://oeis.org/A367940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)= -3*a(n-1) + Σ_{k=1..n}
 *            binomial(n-1,k-1)*4^k*a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the build.  seq_t can be unsigned
 * 8/16/32/64-bit or arbitrary‑precision.  idx_t is an unsigned 64‑bit
 * integer. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* capacity of cache array */
    static size_t len   = 0;   /* number of values currently stored */

    /* Grow cache to hold index n */
    if (n >= cap) {
        size_t new_cap = (cap == 0) ? 16 : cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* If already computed, return the cached value */
    if (len > n) return cache[n];

    for (idx_t i = len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;    /* C(i-1,0) */
        seq_t pow4  = (seq_t)4;    /* 4^1 */

        for (idx_t k = 1; k <= i; ++k) {
            seq_t term = binom;
            term *= pow4;
            term *= cache[i - k];

            sum += term;

            /* Update binomial for next k: C(i-1,k) = C(i-1,k-1) * (i-k) / k */
            if (k < i) {   /* skip as binom(…,i) is 0 after last use */
                binom = binom * (i - k) / k;
                pow4 = pow4 * (seq_t)4;
            }
        }

        /* a(i) = sum - 3 * a(i-1) */
        seq_t sub = (seq_t)3 * cache[i - 1];
        cache[i] = sum - sub;
    }

    return cache[n];
}

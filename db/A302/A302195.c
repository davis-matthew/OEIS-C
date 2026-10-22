/*
 * A302195  Hurwitz inverse of triangular numbers [1,3,6,10,15,...].
 * https://oeis.org/A302195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; for n≥1: a(n) = -∑_{k=1..n} binom(n,k) * (k+1)(k+2)/2 *
 *            a(n−k). This recurrence follows from the EGF 1 / ∑_{m≥0}
 *            (m+1)(m+2)/2 x^m/m!
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 10, i64 n <= 17, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Cache up to a fixed maximum (≈10 000) to avoid dynamic allocation in
   the C++ arbitrary‑precision case. */
#define MAX_TERM 20000

static seq_t cache[MAX_TERM + 1];
static idx_t cache_computed = 0;

seq_t a(idx_t n)
{
    /* initialize base values lazily */
    if (cache_computed == 0) {
        cache[0] = (seq_t)1;
        cache_computed = 0;
        if (n == 0) return cache[0];
        cache[1] = (seq_t)-3;
        cache_computed = 1;
        if (n == 1) return cache[1];
    }

    /* return cached value if already computed */
    if (n <= cache_computed)
        return cache[n];

    /* compute terms up to n */
    for (idx_t m = cache_computed + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;                /* binom(m,0) */
        for (idx_t k = 1; k <= m; ++k) {
            /* C(m,k) = C(m,k-1) * (m-k+1) / k  (exact division) */
            binom = binom * (seq_t)(m - k + 1) / (seq_t)k;
            /* triangular number T_k = (k+1)(k+2)/2 */
            seq_t tk = ((seq_t)(k + 1) * (seq_t)(k + 2)) / (seq_t)2;
            /* accumulate the term */
            seq_t prod = binom * tk * cache[m - k];
            sum = sum + prod;
        }
        cache[m] = -sum;
    }

    cache_computed = n;
    return cache[n];
}

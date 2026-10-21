/*
 * A292909  E.g.f.: exp(1/2! * x^2 * exp(-x)).
 * https://oeis.org/A292909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   The e.g.f. is exp( (x²/2) * e^(–x) ). From combinatorial theory
 *            the coefficients obey a(0) = 1, a(n) = Σ_{k=2}^{n} binom(n‑1,
 *            k‑1) * (k*(k‑1)/2) * (‑1)^k * a(n‑k) for n≥1. This implements
 *            the recurrence derived from the exponential generating function
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 22, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of computed terms */
    static size_t alloc = 0;      /* current allocated size      */
    static idx_t  len   = 0;      /* number of terms stored      */

    /* ensure storage for 0..n */
    size_t needed = (size_t)(n + 1);
    if (needed > alloc) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* out of memory – caller must handle */
        cache = tmp;
        alloc = needed;
    }

    /* initialise first term */
    if (len == 0) {
        cache[0] = (seq_t)1;
        len = 1;
    }

    /* fill in missing terms up to n */
    while (len <= n) {
        idx_t nIdx = len;
        seq_t sum = (seq_t)0;
        seq_t binom = 0;

        if (nIdx >= 2) {
            binom = (seq_t)(nIdx - 1);          /* k = 2 ⇒ binom(n‑1,1) */

            for (idx_t k = 2; k <= nIdx; ++k) {
                if (k > 2) {
                    /* binom(n-1, k-1) = binom(n-1, k-2) * (n-k+1)/(k-1) */
                    binom = binom * (seq_t)(nIdx - k + 1) /
                                   (seq_t)(k - 1);
                }
                seq_t fact_term = (seq_t)(k) * (seq_t)(k - 1) / 2;
                seq_t term = cache[nIdx - k] * binom * fact_term;
                if (k & 1U) term = -term;          /* (‑1)^k for odd k  */
                sum += term;
            }
        }

        cache[len] = sum;
        ++len;
    }

    return cache[n];
}

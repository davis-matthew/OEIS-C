/*
 * A374973  a(n) = Sum_{k=1..n-1} tau(k) * sigma_2(n-k).
 * https://oeis.org/A374973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n-1} tau(k) * sigma_2(n-k)
 * Range:     u8 n <= 7, u16 n <= 37, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A374973: a(n) = Σ_{k=1}^{n-1} τ(k) * σ₂(n-k)
   where τ is the number of divisors and σ₂ is the sum of squares of divisors. */

#include <stdint.h>
#include <stdlib.h>

/* static storage for divisor statistics */
static seq_t *tau = NULL;
static seq_t *sigma2 = NULL;
static size_t max_pre = 0;

/* recompute tau and sigma₂ up to limit (inclusive) */
static void re_compute(idx_t limit)
{
    /* allocate or grow the arrays */
    size_t needed = (size_t)limit + 1;        /* +1 to use indices directly */
    if (needed > max_pre) {
        seq_t *t = (seq_t *)realloc(tau, needed * sizeof(seq_t));
        seq_t *s = (seq_t *)realloc(sigma2, needed * sizeof(seq_t));
        if (!t || !s) {
            /* allocation failed – abort program (not expected in the test harness) */
            abort();
        }
        tau = t;
        sigma2 = s;
        max_pre = needed;
    }

    /* clear data up to limit */
    for (size_t i = 1; i <= (size_t)limit; ++i) {
        tau[i] = 0;
        sigma2[i] = 0;
    }

    /* compute divisor contributions */
    for (idx_t d = 1; d <= limit; ++d) {
        seq_t d2 = (seq_t)d * d;
        for (idx_t m = d; m <= limit; m += d) {
            tau[m]   += 1;
            sigma2[m] += d2;
        }
    }
}

/* -------------------------------------------------------------------------
 * Public sequence function
 * -------------------------------------------------------------------------
 */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;                 /* a(1) = 0, for n==1 exact result */

    /* Ensure divisor tables are ready for indices up to n-1 */
    if (max_pre < (size_t)n) {
        re_compute(n - 1);
    }

    seq_t result = 0;
    for (idx_t k = 1; k < n; ++k) {
        result += tau[k] * sigma2[n - k];
    }
    return result;
}

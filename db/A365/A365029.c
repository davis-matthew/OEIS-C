/*
 * A365029  a(n) = Sum_{k = 0..n} binomial(n+k-1, k)^2 * binomial(2*k-1, n).
 * https://oeis.org/A365029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k = 0..n} binomial(n+k-1, k)^2 * binomial(2*k-1, n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Special case for the offset 0 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Sum over k = 1..n.  The k = 0 term is handled after the loop. */
    seq_t sum = (seq_t)0;

    /* Multiplicative computation of C(n+k-1, k).  t0 = C(n-1,0) = 1. */
    seq_t t = (seq_t)1;

    /* First k for which binomial(2*k-1, n) is non‑zero:
       ceil((n+1)/2).  For k < start the inner binomial vanishes. */
    idx_t start = (n + 1) / 2;

    for (idx_t k = 1; k <= n; ++k) {
        /* Update the binomial coefficient from the previous value. */
        t = (t * (seq_t)(n + k - 1)) / (seq_t)k;

        if (k < start) {
            /* binomial(2*k-1, n) = 0 for these k; skip. */
            continue;
        }

        seq_t f    = binomial(2 * k - 1, n);  /* C(2*k-1, n)   */
        seq_t term = t * t;                   /* t²             */
        term = term * f;                      /* t² * f         */
        sum = sum + term;
    }

    /* Contribution from k = 0 : (-1)^n.  For n odd we subtract 1, for n even we add 1. */
    if (n & 1U) {
        sum = sum - (seq_t)1;
    } else {
        sum = sum + (seq_t)1;
    }

    return sum;
}

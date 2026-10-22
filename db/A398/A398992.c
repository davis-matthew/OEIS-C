/*
 * A398992  The binomial matrix of the central binomial numbers. Square array
 *          read by ascending antidiagonals.
 * https://oeis.org/A398992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the element at index n of the binomial matrix of the
 *            central binomial numbers, read in ascending antidiagonals. For
 *            indices i>=0 and j>=0, the matrix element is A(i,j) = Σ_{m=0..d}
 *            binomial(d,m) * s^(d-m) * CB(p+m), where d = |i-j|, p =
 *            min(i,j), s = 1 if j <= i else -1, and CB(t) = binom...
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool type */
#include "../utils/binomial.h"

static inline seq_t central_binom(idx_t t)
{
    /* Return the central binomial coefficient CB(t) = binomial(2t, t). */
    return binomial(2*t, t);
}

seq_t a(idx_t n)
{
    /* Map sequence index n to anti-diagonal coordinates (i,j). */
    idx_t s = 0;          /* current diagonal sum i+j */
    idx_t cum = 1;        /* cumulative count up to diagonal s: (s+1)(s+2)/2 */
    while (n >= cum) {
        s++;
        cum += (s + 1);   /* next diagonal adds (s+1) elements */
    }

    /* Number of elements before diagonal s */
    idx_t prev_cum = cum - (s + 1);
    idx_t offset = n - prev_cum;   /* position on diagonal s */
    idx_t i = s - offset;
    idx_t j = offset;

    /* Compute A(i,j) using the defining sum. */
    idx_t d = (i > j) ? (i - j) : (j - i);
    idx_t p = (i < j) ? i : j;
    int sgn = (j <= i) ? 1 : -1;   /* s = +1 if j <= i, else -1 */

    seq_t pos = 0;  /* sum of positive terms */
    seq_t neg = 0;  /* sum of negative terms */

    for (idx_t m = 0; m <= d; ++m) {
        seq_t bin_dm = binomial(d, m);
        seq_t cb = central_binom(p + m);

        /* term = binomial(d,m) * CB(p+m) ; sign depends on (d-m) parity */
        seq_t term = bin_dm * cb;
        if ((sgn == 1) || ((d - m) % 2 == 0)) {
            pos += term;
        } else {
            neg += term;
        }
    }

    /* The final value is the difference of the positive and negative sums. */
    return pos - neg;
}

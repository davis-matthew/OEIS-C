/*
 * A356774  Coefficients in the power series expansion of A(x) =
 *          Sum_{n=-oo..+oo} n * x^n * (1 - x^n)^(n-2).
 * https://oeis.org/A356774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{d|n} [ d*binom(d-2, n/d-1)*(-1)^{n/d-1} (if d≥2, n/d ≤
 *            d-1) + d*binom(n/d, d+1)*(-1)^{d+1} (if n/d ≥ d+1) ] plus an
 *            extra +1 from the n=1 term. This follows from splitting the
 *            original definition into positive and negative summands
 * Range:     i8 n <= 11, i16 n <= 67, i32 n <= 311, i64 n <= 1364,
 *            i128 n <= 2050, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the binomial coefficient routine. */
#include "../utils/binom.h"

/* Compute the coefficient a(n) of the power series for A356774. */
seq_t a(idx_t n)
{
    /* The n=1 term supplies a contribution 1 to every coefficient. */
    seq_t res = 1;

    /* Iterate over all positive divisors d of n. */
    for (idx_t d = 1; d*d <= n; ++d) {
        if (n % d != 0) continue;

        /* First divisor: d. */
        idx_t q = n / d;
        /* Positive-n contribution (d≥2). */
        if (d >= 2 && q <= d-1) {
            seq_t sign = ((q-1) & 1) ? -1 : 1; /* (-1)^(q-1) */
            seq_t bin  = binom(d-2, q-1);
            seq_t term = (seq_t)d * bin;
            res += (sign == 1) ? term : -term;
        }
        /* Negative-n contribution. */
        if (q >= d+1) {
            seq_t sign = ((d+1) & 1) ? -1 : 1; /* (-1)^(d+1) */
            seq_t bin  = binom(q, d+1);
            seq_t term = (seq_t)d * bin;
            res += (sign == 1) ? term : -term;
        }

        /* Second divisor: n/d, avoid double‑counting when d*d==n. */
        if (d != n / d) {
            idx_t d2 = n / d;          /* divisor n/d */
            idx_t q2 = d;              /* because n / (n/d) = d */

            /* Positive-n contribution (d2≥2). */
            if (d2 >= 2 && q2 <= d2-1) {
                seq_t sign = ((q2-1) & 1) ? -1 : 1;
                seq_t bin  = binom(d2-2, q2-1);
                seq_t term = (seq_t)d2 * bin;
                res += (sign == 1) ? term : -term;
            }
            /* Negative-n contribution. */
            if (q2 >= d2+1) {
                seq_t sign = ((d2+1) & 1) ? -1 : 1;
                seq_t bin  = binom(q2, d2+1);
                seq_t term = (seq_t)d2 * bin;
                res += (sign == 1) ? term : -term;
            }
        }
    }

    return res;
}

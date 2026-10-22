/*
 * A326607  G.f.: Sum_{n>=0} (2*n+1) * x^n * (1 - x^n)^n.
 * https://oeis.org/A326607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} (2*d+1) * (-1)^{(n/d)-1} * binom(d, n/d-1),
 *            where binom(d,k)=0 if k>d or k<0. a(0) = 1. The summation runs
 *            over all positive divisors d of n; only terms with n/d <= d+1
 *            contribute (otherwise binom(d, n/d-1)=0)
 * Range:     i8 n <= 14, i16 n <= 71, i32 n <= 314, i64 n <= 1399,
 *            i128 n <= 2550, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* Multiply then divide; intermediate may overflow for very
           large arguments, but for values where a(n) fits in seq_t
           the operations stay within range.  */
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* a(0) = 1 */

    seq_t sum = 0;

    /* Iterate over all divisors d of n without overflow. */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0) continue;

        /* First divisor d.  */
        idx_t t = n / d;           /* t = n/d */
        if (t <= d + 1) {
            seq_t bin = binom(d, t - 1);
            seq_t term = (seq_t)(2 * d + 1) * bin;
            if ((t - 1) & 1) term = -term;
            sum += term;
        }

        /* Paired divisor n/d (if distinct). */
        idx_t d2 = n / d;
        if (d2 != d) {
            idx_t t2 = n / d2;     /* t2 = d */
            if (t2 <= d2 + 1) {
                seq_t bin = binom(d2, t2 - 1);
                seq_t term = (seq_t)(2 * d2 + 1) * bin;
                if ((t2 - 1) & 1) term = -term;
                sum += term;
            }
        }
    }

    return sum;
}

/*
 * A280022  Expansion of phi_{5, 4}(x) where phi_{r, s}(x) = Sum_{n, m>0} m^r
 *          * n^s * x^{m*n}.
 * https://oeis.org/A280022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n^4 * sigma(n) for n > 0, a(0) = 0
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 71, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 48 terms through n = 818 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t nn   = (seq_t)n;          /* n */
    seq_t n2   = nn * nn;           /* n^2 */
    seq_t n4   = n2 * n2;           /* n^4 */
    seq_t sigma = sigma_of(n);      /* sigma_1(n) */

    return n4 * sigma;
}

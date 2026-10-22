/*
 * A364351  Expansion of Sum_{k>0} k^2 * x^k/(1 + x^k)^3.
 * https://oeis.org/A364351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n/2) * Σ_{d|n} (-1)^(n/d+1) * (d + n)
 * Range:     i8 n <= 8, i16 n <= 95, i32 n <= 9359, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t nn = n;

    /* iterate over divisors d of n */
    for (idx_t d = 1; d <= nn / d; ++d) {
        if (nn % d == 0) {
            /* first divisor d */
            seq_t sign = ((nn / d + 1) & 1) ? (seq_t)(-1) : (seq_t)1;
            sum += sign * ((seq_t)d + (seq_t)nn);

            /* complementary divisor nn / d, if distinct */
            idx_t d2 = nn / d;
            if (d2 != d) {
                seq_t sign2 = ((nn / d2 + 1) & 1) ? (seq_t)(-1) : (seq_t)1;
                sum += sign2 * ((seq_t)d2 + (seq_t)nn);
            }
        }
    }

    return ((seq_t)n * sum) / 2;
}

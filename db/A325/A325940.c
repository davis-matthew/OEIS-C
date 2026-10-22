/*
 * A325940  Expansion of Sum_{k>=1} x^(2*k) / (1 + x^k)^2.
 * https://oeis.org/A325940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = Σ_{d|n} (-1)^d · (d-1) (OEIS A325940)
 * Range:     i8 n <= 71, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;

    /* Loop over divisors d up to sqrt(n).  For each divisor we process
       the complementary divisor n/d as well. */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            /* process divisor d */
            seq_t term_d = ((d & 1) ? -1 : 1) * ((seq_t)d - 1);
            result += term_d;

            /* process complementary divisor n/d if distinct */
            idx_t d2 = n / d;
            if (d2 != d) {
                seq_t term_d2 = ((d2 & 1) ? -1 : 1) * ((seq_t)d2 - 1);
                result += term_d2;
            }
        }
    }
    return result;
}

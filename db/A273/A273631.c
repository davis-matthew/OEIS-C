/*
 * A273631  a(n) = Sum_{k = 0..n} (-1)^k*binomial(k,2)^3*binomial(n,k)^3.
 * https://oeis.org/A273631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 0, a(2m+1) = 0; a(2m+2) = (-1)^m * (m+1)^3 * (2m+1)^3 *
 *            binomial(3m, m) * binomial(2m, m) for m >= 0 where binomial(n,
 *            k) is the ordinary binomial coefficient. This is equivalent to
 *            the given definition: a(n) = Σ_{k=0..n}
 *            (-1)^k*binomial(k,2)^3*binomial(n,k)^3
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 21, i128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) is defined as 0 */
    if (n == 0)
        return 0;

    /* odd indices give 0 */
    if (n & 1)
        return 0;

    /* n is even and >=2: let m = (n-2)/2 */
    idx_t m = (n >> 1) - 1;   /* (n/2) - 1 */

    /* (m+1)^3 */
    seq_t m1 = (seq_t)(m + 1);
    seq_t coeff = m1 * m1 * m1;

    /* (2m+1)^3 */
    idx_t two_m_plus1_idx = 2 * m + 1;
    seq_t two_m_plus1 = (seq_t)two_m_plus1_idx;
    coeff = coeff * two_m_plus1 * two_m_plus1 * two_m_plus1;

    /* apply the sign (-1)^m */
    if (m & 1)
        coeff = -coeff;

    /* binomial factors */
    seq_t b1 = binomial(3 * m, m);
    seq_t b2 = binomial(2 * m, m);

    /* result */
    seq_t result = coeff * b1 * b2;
    return result;
}

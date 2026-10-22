/*
 * A364045  Expansion of Sum_{k>0} x^(3*k) / (1 + x^(5*k)).
 * https://oeis.org/A364045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{d|n, d ≡ 3 (mod 5)} (-1)^{(d-3)/5}
 * Range:     i8 n <= 102, i16 n <= 102, i32 n <= 102, i64 n <= 102,
 *            i128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            /* divisor d */
            if (d % 5 == 3)
            {
                idx_t exp = (d - 3) / 5;          /* exponent for sign */
                sum += (exp & 1) ? -1 : 1;        /* (-1)^exp */
            }

            /* paired divisor n/d */
            idx_t d2 = n / d;
            if (d2 != d && d2 % 5 == 3)
            {
                idx_t exp2 = (d2 - 3) / 5;
                sum += (exp2 & 1) ? -1 : 1;
            }
        }
    }
    return sum;
}

/*
 * A363630  Expansion of Sum_{k>0} (1/(1+x^k)^3 - 1).
 * https://oeis.org/A363630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{d | n} (-1)^d * binomial(d+2,2)
 * Range:     i8 n <= 14, i16 n <= 215, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation using divisor summation.   */
seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* binomial(d+2,2) = (d+2)*(d+1)/2 */
            seq_t term1 = (seq_t)(d1 + 2) * (d1 + 1) / 2;
            if (d1 & 1) total -= term1;
            else        total += term1;

            if (d1 != d2)
            {
                seq_t term2 = (seq_t)(d2 + 2) * (d2 + 1) / 2;
                if (d2 & 1) total -= term2;
                else        total += term2;
            }
        }
    }
    return total;
}

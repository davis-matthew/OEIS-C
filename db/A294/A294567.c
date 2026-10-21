/*
 * A294567  a(n) = Sum_{d|n} d^(1 + 2*n/d).
 * https://oeis.org/A294567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(1 + 2*n/d)
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 29, u64 n <= 59, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* outside the offset, arbitrary value */

    seq_t sum = 0;

    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            /* first divisor d */
            idx_t exp1 = 1 + (2 * n) / d;
            seq_t base1 = (seq_t)d;
            seq_t pow1  = 1;
            for (idx_t k = 0; k < exp1; ++k)
                pow1 *= base1;
            sum += pow1;

            idx_t other = n / d;
            if (other != d)
            {
                idx_t exp2 = 1 + (2 * n) / other;
                seq_t base2 = (seq_t)other;
                seq_t pow2  = 1;
                for (idx_t k = 0; k < exp2; ++k)
                    pow2 *= base2;
                sum += pow2;
            }
        }
    }

    return sum;
}

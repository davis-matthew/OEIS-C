/*
 * A364205  Expansion of Sum_{k>=0} x^(3*k+2) / (1 + x^(3*k+2)).
 * https://oeis.org/A364205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n, n/d==2 (mod 3)} (-1)^(d+1)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            /* divisor d1 = i */
            idx_t d1 = i;
            idx_t t1 = n / d1;
            if (t1 % 3 == 2)
                sum += (d1 & 1) ? 1 : -1;

            /* paired divisor d2 = n / i */
            idx_t d2 = n / i;
            if (d2 != d1)
            {
                idx_t t2 = n / d2; /* equals i */
                if (t2 % 3 == 2)
                    sum += (d2 & 1) ? 1 : -1;
            }
        }
    }
    return sum;
}

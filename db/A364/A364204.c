/*
 * A364204  Expansion of Sum_{k>=0} x^(3*k+1) / (1 + x^(3*k+1)).
 * https://oeis.org/A364204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n, n/d == 1 (mod 3)} (-1)^(d+1)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t e = n / d;
            if (e % 3 == 1)
            {
                seq_t sign = (d & 1) ? (seq_t)1 : (seq_t)(-1);
                sum += sign;
            }
            if (d != e)
            {
                if (d % 3 == 1)
                {
                    seq_t sign2 = (e & 1) ? (seq_t)1 : (seq_t)(-1);
                    sum += sign2;
                }
            }
        }
    }
    return sum;
}

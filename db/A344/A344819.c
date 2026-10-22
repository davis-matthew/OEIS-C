/*
 * A344819  a(n) = Sum_{k=1..n} floor(n/k) * (-4)^(k-1).
 * https://oeis.org/A344819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} floor(n/k) * (-4)^(k-1)
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 16, i64 n <= 32, i128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t pow = 1;                      /* (-4)^(k-1)  when k == 1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t t = (seq_t)(n / k);       /* floor(n/k) */
        sum += t * pow;                 /* accumulate term */
        pow *= (seq_t)-4;               /* update power for next k */
    }

    return sum;
}

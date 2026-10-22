/*
 * A346502  a(n) = 3n - (sum of digits of 3n in base 3).
 * https://oeis.org/A346502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 3*n - sum of digits of 3*n in base 3
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * (seq_t)3;   /* m = 3*n */
    seq_t temp = m;
    seq_t sum  = 0;
    while (temp != 0)
    {
        sum += temp % (seq_t)3;
        temp /= (seq_t)3;
    }
    return m - sum;
}

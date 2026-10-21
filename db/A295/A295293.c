/*
 * A295293  Sum of the products of the smaller and larger parts of the
 *          partitions of n into two parts with the larger part even.
 * https://oeis.org/A295293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..⌊n/2⌋} i*(n-i) if (n-i) is even
 * Range:     u8 n <= 18, u16 n <= 115, u32 n <= 4687, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t half = n >> 1;          /* floor(n/2) */
    for (idx_t i = 1; i <= half; ++i)
    {
        idx_t m = n - i;          /* larger part */
        if ((m & 1) == 0)         /* m even */
        {
            sum += (seq_t)i * (seq_t)m;
        }
    }
    return sum;
}

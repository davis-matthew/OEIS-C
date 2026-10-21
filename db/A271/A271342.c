/*
 * A271342  Sum of all even divisors of all positive integers <= n.
 * https://oeis.org/A271342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{d even, 2 <= d <= n} d * floor(n/d)
 * Range:     u8 n <= 25, u16 n <= 399, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t d = 2; d <= n; ++d)
    {
        if ((d & 1) == 0)          /* d is even */
        {
            idx_t cnt = n / d;     /* how many multiples of d up to n */
            total += (seq_t)d * (seq_t)cnt;
        }
    }
    return total;
}

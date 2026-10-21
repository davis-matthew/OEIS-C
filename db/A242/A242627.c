/*
 * A242627  Number of divisors of n that are less than 10.
 * https://oeis.org/A242627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of positive divisors of n that are < 10, with a(0)
 *            = 9
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)9;

    seq_t cnt = 0;
    for (idx_t d = 1; d <= 9; ++d)
    {
        if (n % d == 0)
            ++cnt;
    }
    return cnt;
}

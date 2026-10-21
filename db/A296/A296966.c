/*
 * A296966  Sum of all the parts in the partitions of n into two distinct
 *          parts such that the smaller part divides the larger.
 * https://oeis.org/A296966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * sum_{i=1}^{⌊(n-1)/2⌋} (⌊n/i⌋ - ⌊(n-1)/i⌋) Equivalent:
 *            a(n) = n * ( number of proper divisors of n that are < n/2 )
 * Range:     u8 n <= 47, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count divisors of n (τ(n)) */
    idx_t tot = 0;
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            ++tot;
            if (i != n / i)
                ++tot;
        }
    }

    /* Proper divisors count */
    idx_t count = tot - 1;
    if ((n & 1) == 0)          /* exclude n/2 when n is even */
        count -= 1;

    return (seq_t)n * (seq_t)count;
}

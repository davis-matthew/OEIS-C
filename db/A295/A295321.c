/*
 * A295321  Sum of the products of the smaller and larger parts of the
 *          partitions of n into two distinct parts with the larger part even.
 * https://oeis.org/A295321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor((n-1)/2)} i * (n-i) if (n-i) is even
 * Range:     u8 n <= 18, u16 n <= 116, u32 n <= 4689, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*        Implementation directly follows the OEIS definition. */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t limit = (n - 1) / 2;          /* floor((n-1)/2) */
    for (idx_t i = 1; i <= limit; ++i)
    {
        idx_t L = n - i;                /* larger part of the partition */
        if ((L & 1U) == 0)              /* L is even */
        {
            sum += (seq_t)i * (seq_t)L;
        }
    }
    return sum;
}

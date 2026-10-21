/*
 * A275535  a(n) = the smallest positive multiple of n that is the sum of more
 *          than 1 consecutive positive integers.
 * https://oeis.org/A275535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n is not a power of 2, 3*n if n is a power of 2
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 */
    if ((n & (n - 1)) == 0)      /* power of two check */
        return (seq_t)n * 3;
    else
        return (seq_t)n;
}

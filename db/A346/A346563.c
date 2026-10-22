/*
 * A346563  a(n) = n + A007978(n).
 * https://oeis.org/A346563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + A007978(n) where A007978(n) is the least integer m >
 *            1 that does not divide n
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of range, not used by the sequence */
    idx_t m = 2;
    while (n % m == 0)      /* find the smallest m > 1 not dividing n */
        m++;
    return (seq_t)n + (seq_t)m;
}

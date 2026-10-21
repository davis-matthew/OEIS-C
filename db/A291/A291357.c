/*
 * A291357  The arithmetic function u(n,2,3).
 * https://oeis.org/A291357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 if n is even; 3 if n is odd and divisible by 3; 4
 *            otherwise
 * Range:     u8 n <= 9617, u16 n <= 9617, u32 n <= 9617, u64 n <= 9617,
 *            u128 n <= 9617, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* even n */
    if ((n & 1ULL) == 0)
        return (seq_t)2;
    /* odd n divisible by 3 */
    if (n % 3 == 0)
        return (seq_t)3;
    /* remaining odd n */
    return (seq_t)4;
}

/*
 * A276764  1^2 + 3^2, 2^2 + 4^2, 5^2 + 7^2, 6^2 + 8^2, ...
 * https://oeis.org/A276764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^2 + 2 if n is odd a(n) = 8*n^2 - 8*n + 4 if n is even
 * Range:     u8 n <= 6, u16 n <= 90, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n & 1ULL) == 0)    /* even n */
        return 8 * nn * nn - 8 * nn + 4;
    else                    /* odd n */
        return 8 * nn * nn + 2;
}

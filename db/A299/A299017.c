/*
 * A299017  Intersection of A264041 and A000217.
 * https://oeis.org/A299017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2*n^2 - 7*n + 6 for n>3; a(1)=1, a(2)=3, a(3)=6
 * Range:     u8 n <= 13, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3)
    {
        if (n == 1) return (seq_t)1;
        if (n == 2) return (seq_t)3;
        return (seq_t)6; /* n == 3 */
    }

    seq_t nn = (seq_t)n;
    return 2 * nn * nn - 7 * nn + 6;
}

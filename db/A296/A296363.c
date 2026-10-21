/*
 * A296363  a(1)=0; for n>1, a(n) = 4*n^3 - 3*n^2 - 3*n + 4.
 * https://oeis.org/A296363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^3 - 3*n^2 - 3*n + 4
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1024, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;
    seq_t nn = n;
    seq_t term = 4 * nn * nn * nn - 3 * nn * nn - 3 * nn + 4;
    return term;
}

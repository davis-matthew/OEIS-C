/*
 * A299279  Coordination sequence for "reo" 3D uniform tiling.
 * https://oeis.org/A299279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(1)=8; for n>1: a(n)=8*n^2-2 if n even, a(n)=7*n^2+5 if
 *            n odd
 * Range:     u8 n <= 5, u16 n <= 91, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)8;

    seq_t nn = (seq_t)n;
    if (n & 1)    /* odd */
        return (seq_t)(7 * nn * nn + 5);
    else          /* even */
        return (seq_t)(8 * nn * nn - 2);
}

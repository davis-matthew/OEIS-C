/*
 * A393957  Number of bipartite non-complete simple games with a maximal
 *          number of minimal winning pairs.
 * https://oeis.org/A393957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n<4; a(n) = (n*(n-2))/8 if n even; a(n) =
 *            ((n-3)*(n^2+4*n-13))/16 if n odd
 * Range:     u8 n <= 14, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;

    /* Cast n to seq_t for all arithmetic */
    seq_t nn = (seq_t)n;

    if ((n & 1) == 0)          /* even n */
    {
        /* a(n) = n*(n-2)/8 */
        return (nn * (nn - 2)) / 8;
    }
    else
    {
        /* a(n) = (n-3)*(n^2+4*n-13)/16 */
        seq_t term = nn * nn + 4 * nn - 13;
        return ((nn - 3) * term) / 16;
    }
}

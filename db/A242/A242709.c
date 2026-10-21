/*
 * A242709  Nonequivalent ways to place two different markers (e.g., a pair of
 *          Go stones, black and white) on an n X n grid.
 * https://oeis.org/A242709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^3 + 3*n - 4)/8 for odd n, a(n) = n*(n^3 + n - 2)/8
 *            for even n
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1; n=0 is unused */
    seq_t nn = (seq_t)n;                /* cast first to avoid overflow */
    seq_t n2  = nn * nn;                /* n^2  */
    seq_t n3  = n2 * nn;                /* n^3  */
    seq_t res;

    if ((n & 1) == 0) {                 /* n is even */
        res = nn * (n3 + nn - (seq_t)2);
    } else {                            /* n is odd  */
        res = nn * (n3 + (seq_t)3 * nn - (seq_t)4);
    }
    return res / (seq_t)8;              /* division by 8 is exact */
}

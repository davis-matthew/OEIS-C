/*
 * A270257  Number of 2X2X2 triangular 0..n arrays with some element plus some
 *          adjacent element totalling n exactly once.
 * https://oeis.org/A270257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n^2 if n is even * a(n) = 3*n^2 - 3 if n is odd *
 *            (Empirical formulas listed in the OEIS entry)
 * Range:     u8 n <= 9, u16 n <= 147, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = 3 * nn * nn;
    if ((n & 1) != 0)          /* odd n */
        res -= 3;
    return res;
}

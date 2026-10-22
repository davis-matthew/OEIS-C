/*
 * A338284  a(n) is the smallest nonsquare m such that the second partial
 *          quotient in the continued fraction for sqrt(m) equals n.
 * https://oeis.org/A338284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n/2)^2 + 1 if n is even n^2 + 4*n + 2 if n is odd
 * Range:     u8 n <= 14, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {                  /* even n */
        seq_t half = (seq_t)(n / 2);
        return half * half + 1;
    } else {                              /* odd n */
        seq_t nn = (seq_t)n;
        return nn * nn + 4 * nn + 2;
    }
}

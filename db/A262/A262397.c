/*
 * A262397  a(n) = floor(A261327(n)/9).
 * https://oeis.org/A262397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = floor((n^2+4)/36) when n is even, a(n) = floor((n^2+4)/9)
 *            when n is odd
 * Range:     u8 n <= 48, u16 n <= 768, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n & 1u) == 0)              /* even n */
        return (nn * nn + 4) / 36;
    else                            /* odd n */
        return (nn * nn + 4) / 9;
}

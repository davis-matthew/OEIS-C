/*
 * A329279  Number of distinct tilings of a 2n X 2n square with 1 x n
 *          polyominoes.
 * https://oeis.org/A329279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for n=1, a(1)=1; otherwise, a(n) = (n^2 + 4n)/2 + 3 if n is
 *            even, a(n) = (n^2 + 3n)/2 + 2 if n is odd
 * Range:     u8 n <= 21, u16 n <= 360, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t nseq = (seq_t)n;
    seq_t res;

    if ((n & 1) == 0) { /* even n */
        res = (nseq * nseq + (seq_t)4 * nseq) / (seq_t)2;
        res += (seq_t)3;
    } else { /* odd n */
        res = (nseq * nseq + (seq_t)3 * nseq) / (seq_t)2;
        res += (seq_t)2;
    }

    return res;
}

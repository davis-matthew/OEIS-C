/*
 * A294286  Sum of the squares of the parts in the partitions of n into two
 *          distinct parts.
 * https://oeis.org/A294286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 - 3*n^2 + n)/6 for odd n; a(n) = (4*n^3 - 9*n^2 +
 *            2*n)/12 for even n
 * Range:     u8 n <= 5, u16 n <= 27, u32 n <= 1422, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;          /* convert index to seq_t */
    seq_t n2   = nseq * nseq;       /* n^2  */
    seq_t n3   = n2 * nseq;         /* n^3  */
    seq_t result;

    if (n & 1) { /* odd n */
        seq_t num = 2 * n3 - 3 * n2 + nseq;
        result = num / 6;
    } else {      /* even n */
        seq_t num = 4 * n3 - 9 * n2 + 2 * nseq;
        result = num / 12;
    }
    return result;
}

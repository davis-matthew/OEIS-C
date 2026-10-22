/*
 * A370980  If n is even, (n^2-2*n+2)/2, otherwise (n^2-n+2)/2.
 * https://oeis.org/A370980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + n*floor((n-1)/2)
 * Range:     u8 n <= 23, u16 n <= 362, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;                  /* cast index to seq_t */
    return 1 + nseq * ((nseq - 1) / 2);     /* exact arithmetic in seq_t */
}

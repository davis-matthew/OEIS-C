/*
 * A394826  The tetrahemihexahedral numbers.
 * https://oeis.org/A394826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 2)*(4*n^2 - 5*n + 3)/6
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t term = (nseq + 2) *
                 ( (seq_t)4 * nseq * nseq - (seq_t)5 * nseq + (seq_t)3 ) / (seq_t)6;
    return term;
}

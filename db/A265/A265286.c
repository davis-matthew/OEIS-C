/*
 * A265286  Minimal number of pieces of a cake such that they can be
 *          distributed equally among k guests for any k=1,2,...,n.
 * https://oeis.org/A265286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, more, hard
 * Formula:   a(n) = floor((5*n-7)/2) for n >= 3, with a(1) = 1, a(2) = 2
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    seq_t tn = (seq_t)n;
    return ((seq_t)5 * tn - (seq_t)7) / (seq_t)2;
}

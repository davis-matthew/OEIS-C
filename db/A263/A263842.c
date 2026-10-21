/*
 * A263842  Maximal Hamming distance of a doubly-even self-dual code of length
 *          4n over GF(4).
 * https://oeis.org/A263842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 3 for n=1; a(n) = 2*floor((n+3)/2) for n>=2
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) {
        return (seq_t)3;
    }
    seq_t tmp = (seq_t)n + ((seq_t)3);
    tmp = tmp / ((seq_t)2);
    return tmp * ((seq_t)2);
}

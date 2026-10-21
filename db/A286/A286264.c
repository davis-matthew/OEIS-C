/*
 * A286264  a(n) = 2*(ceiling((n^2)/2)+1) - 1.
 * https://oeis.org/A286264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + 1 if n is even, n^2 + 2 if n is odd
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;
    if ((n & 1u) == 0)
        sq += 1;
    else
        sq += 2;
    return sq;
}

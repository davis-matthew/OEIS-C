/*
 * A277792  Squares that are also pentagonal pyramidal numbers.
 * https://oeis.org/A277792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * (2*n^2 - 1)^2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 32, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;          /* n^2 */
    seq_t t  = ((seq_t)2 * sq) - 1;  /* 2*n^2 - 1 */
    return sq * t * t;
}

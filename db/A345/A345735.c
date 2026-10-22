/*
 * A345735  A prime-generating quasipolynomial: a(n) = 6*floor(n^2/4) + 17.
 * https://oeis.org/A345735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*floor(n^2/4) + 17
 * Range:     u8 n <= 12, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;       /* n^2, safe for all supported sizes as long as a(n) fits */
    seq_t f  = sq / 4;        /* floor(n^2/4) */
    return 6 * f + 17;
}

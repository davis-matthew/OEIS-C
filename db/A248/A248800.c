/*
 * A248800  a(n) = (2*n^2 + 3 + (-1)^n)/2.
 * https://oeis.org/A248800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^2 + 3 + (-1)^n)/2
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;
    if ((n & 1ULL) == 0)
        return sq + 2; /* even n: n^2 + 2 */
    else
        return sq + 1; /* odd n:  n^2 + 1 */
}

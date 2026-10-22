/*
 * A367204  Number of diagonals in a regular n-gon that are parallel to an
 *          edge.
 * https://oeis.org/A367204
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-3)/2 if n is odd * a(n) = n*(n-4)/4 if n is even *
 *            (Offset: 3, so a(3) is the first term)
 * Range:     u8 n <= 24, u16 n <= 364, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 3. For safety, return 0 for n < 3. */
    if (n < 3u) return 0;

    seq_t N = (seq_t)n;
    if ((n & 1u) == 1u) /* odd n */
        return N * (N - 3) / 2;
    else               /* even n */
        return N * (N - 4) / 4;
}

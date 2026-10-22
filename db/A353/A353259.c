/*
 * A353259  Solution to Snake Numbers Problems for Snakes from 1 to n for an n
 *          X n square grid (see Comments).
 * https://oeis.org/A353259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n^2 for even n; a(1) = 1; for odd n > 1, a(n) = (n−1)^2 +
 *            3
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* not part of the official offset, defensive */
    if (n == 1)
        return (seq_t)1;

    seq_t tn = (seq_t)n;
    if ((n & 1) == 0)               /* even n */
        return tn * tn;
    else                              /* odd n > 1 */
    {
        seq_t t = tn - 1;               /* t = n−1 */
        return t * t + (seq_t)3;
    }
}

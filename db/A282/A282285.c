/*
 * A282285  Least common multiple of 5*n+1 and 5*n-1.
 * https://oeis.org/A282285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(5n-1, 5n+1) * a(0) = 1 * a(n) = 25*n^2 - 1 for n > 0
 *            even * a(n) = (25*n^2 - 1)/2 for n odd
 * Range:     u8 n <= 3, u16 n <= 51, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* special case */

    seq_t nn = (seq_t)n;          /* convert index to seq_t */
    seq_t sq = nn * nn;           /* n^2 */
    seq_t val = sq * 25 - 1;      /* 25*n^2 - 1 */

    if (n & 1)                    /* odd index: divide by 2 */
        val /= 2;

    return val;
}

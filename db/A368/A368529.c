/*
 * A368529  a(n) = Sum_{k=1..n} k^2 * 4^(n-k).
 * https://oeis.org/A368529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1..n} k^2 * 4^(n-k) Equivalent recurrence: a(0) =
 *            0, a(n) = 4*a(n-1) + n^2 for n > 0
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t sq   = (seq_t)i * (seq_t)i;   /* i^2 */
        result = result * (seq_t)4 + sq;    /* 4*a + i^2 */
    }
    return result;
}

/*
 * A301860  Number of steps required in the worst case for n knights to find
 *          the princess in a castle with rooms arranged in a (n+1) X (n+1)
 *          grid (Castle and princess puzzle).
 * https://oeis.org/A301860
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2*n + 4 for n >= 4; a(2) = 10; a(3) = 12
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 2) return (seq_t)10;
    if (n == 3) return (seq_t)12;
    seq_t nn = (seq_t)n;
    return nn * 2 + 4;
}

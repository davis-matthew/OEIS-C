/*
 * A294116  Fibonacci sequence beginning 2, 21.
 * https://oeis.org/A294116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), with a(0) = 2, a(1) = 21
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)21;

    seq_t prev_prev = (seq_t)2;
    seq_t prev      = (seq_t)21;
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t cur = prev_prev + prev;
        prev_prev = prev;
        prev      = cur;
    }
    return prev;
}

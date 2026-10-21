/*
 * A294157  Fibonacci sequence beginning 2, 8.
 * https://oeis.org/A294157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 2, a(1) = 8
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)8;

    seq_t prev = (seq_t)2;
    seq_t cur  = (seq_t)8;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = prev + cur;
        prev = cur;
        cur  = next;
    }
    return cur;
}

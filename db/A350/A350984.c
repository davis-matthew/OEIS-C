/*
 * A350984  a(0)=0, a(1)=18, a(2)=612; a(n) = 35*(a(n-1)-a(n-2))+a(n-3).
 * https://oeis.org/A350984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 34*a(n-1) - a(n-2), a(0)=0, a(1)=18
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)18;

    seq_t prev_prev = (seq_t)0;   /* a(n-2) */
    seq_t prev      = (seq_t)18;  /* a(n-1) */
    seq_t cur = prev;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)34 * prev - prev_prev;
        prev_prev = prev;
        prev = cur;
    }
    return cur;
}

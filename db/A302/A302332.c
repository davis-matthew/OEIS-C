/*
 * A302332  a(0)=1, a(1)=193; for n>1, a(n) = 194*a(n-1) - a(n-2).
 * https://oeis.org/A302332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(1) = 193 for n > 1: a(n) = 194*a(n-1) - a(n-2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)193;
    seq_t prevprev = (seq_t)1;   /* a(0) */
    seq_t prev = (seq_t)193;     /* a(1) */
    seq_t curr;
    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)194 * prev - prevprev;
        prevprev = prev;
        prev = curr;
    }
    return prev;
}

/*
 * A291181  p-INVERT of the positive integers, where p(S) = 1 - 8*S.
 * https://oeis.org/A291181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 8, a(1) = 80, a(n) = 10*a(n-1) - a(n-2) (n >= 2)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)8;
    if (n == 1) return (seq_t)80;

    /* Iterate the linear recurrence up to the desired index. */
    seq_t prev2 = (seq_t)8;   /* a(n-2) */
    seq_t prev1 = (seq_t)80;  /* a(n-1) */
    seq_t cur    = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)10 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}

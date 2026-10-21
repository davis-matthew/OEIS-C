/*
 * A270889  Integers n such that the circular graph C_n has a square size
 *          deficiency.
 * https://oeis.org/A270889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+2) = 6*a(n+1) - a(n) - 6, a(0)=3, a(1)=6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)3;
    if (n == 1)
        return (seq_t)6;

    seq_t prev2 = (seq_t)3;   /* a(0) */
    seq_t prev1 = (seq_t)6;   /* a(1) */
    seq_t curr  = 0;

    for (idx_t i = 2; i <= n; ++i) {
        curr = 6 * prev1 - prev2 - (seq_t)6;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}

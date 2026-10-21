/*
 * A280261  Period 12 sequence [0, 1, -1, 0, -1, -1, 0, -1, 1, 0, 1, 1, ...].
 * https://oeis.org/A280261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^(n-1)*a(n-1) + a(n-2), a(0) = 0, a(1) = 1
 * Range:     i8 n <= 83, i16 n <= 83, i32 n <= 83, i64 n <= 83, i128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    seq_t prev2 = (seq_t)0;   /* a(0) */
    seq_t prev1 = (seq_t)1;   /* a(1) */
    seq_t curr;
    for (idx_t i = 2; i <= n; ++i)
    {
        /* (-1)^(i-1) : +1 when i is odd, -1 when i is even */
        seq_t mult = (seq_t)((i & 1) ? 1 : -1);
        curr = mult * prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}

/*
 * A256233  a(n) = L(2*n+1) - 2, where L is A000032.
 * https://oeis.org/A256233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = L(2*n+1) - 2 Recurrence: a(0) = -1, a(1) = 2, a(2) = 9,
 *            a(n) = 4*a(n-1) - 4*a(n-2) + a(n-3) for n ≥ 3
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 21, i64 n <= 44, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)(-1);
    if (n == 1) return (seq_t)(2);
    if (n == 2) return (seq_t)(9);

    seq_t prev2 = (seq_t)(-1); /* a(n-3)  */
    seq_t prev1 = (seq_t)(2);  /* a(n-2)  */
    seq_t cur   = (seq_t)(9);  /* a(n-1)  */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = 4 * cur - 4 * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
        cur = next;
    }
    return cur; /* a(n) */
}

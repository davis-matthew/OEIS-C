/*
 * A321133  a(n) = 3*a(n-1) + 10*a(n-2), n >= 2; a(0)=-1, a(1)=23.
 * https://oeis.org/A321133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = -1, a(1) = 23 Recurrence: a(n) = 3*a(n-1) + 10*a(n-2) for
 *            n >= 2 (OEIS entry A321133)
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 12, i64 n <= 26, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)-1;
    if (n == 1) return (seq_t)23;

    seq_t prev2 = (seq_t)-1;   /* a(n-2) */
    seq_t prev1 = (seq_t)23;   /* a(n-1) */
    seq_t next;

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 3*a(i-1) + 10*a(i-2) */
        next = (seq_t)3 * prev1 + (seq_t)10 * prev2;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;
}

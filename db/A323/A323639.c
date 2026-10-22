/*
 * A323639  a(n) = 3*(10^n - 4)/9.
 * https://oeis.org/A323639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*(10^n - 4)/9
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 9, i64 n <= 19, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = -1, a(1) = 2, a(n) = 11*a(n-1) - 10*a(n-2) for n ≥ 2 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)-1;
    if (n == 1) return (seq_t)2;

    seq_t a_prev2 = (seq_t)-1; /* a(0) */
    seq_t a_prev1 = (seq_t)2;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = 11 * a_prev1 - 10 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = cur;
    }

    return a_prev1;
}

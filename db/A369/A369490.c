/*
 * A369490  a(n) = 3^(n+1) + 2*(-2)^(n+1).
 * https://oeis.org/A369490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3^(n+1) + 2*(-2)^(n+1); Recurrence: a(0) = -1, a(1) = 17,
 *            a(n) = a(n-1) + 6*a(n-2) for n >= 2
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 18, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Base cases */
    if (n == 0) return (seq_t)-1;
    if (n == 1) return (seq_t)17;

    seq_t prev2 = (seq_t)-1;   /* a(0) */
    seq_t prev1 = (seq_t)17;   /* a(1) */
    seq_t current = 0;
    idx_t i;

    /* Iteratively compute a(2) ... a(n) */
    for (i = 2; i <= n; ++i) {
        current = prev1 + (seq_t)6 * prev2;
        prev2 = prev1;
        prev1 = current;
    }
    return prev1;
}

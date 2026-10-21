/*
 * A292696  a(n) = L(n)^2 - 5*(-1)^n = L(n+1)*L(n-1), where L = A000032.
 * https://oeis.org/A292696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3) Initial values: a(0) = -1,
 *            a(1) = 6, a(2) = 4
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 22, i64 n <= 45, i128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three indices directly. */
    if (n == 0) return (seq_t)-1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)4;

    /* Use a sliding window of the last three terms. */
    seq_t a0 = (seq_t)-1;   /* a(n-3) */
    seq_t a1 = (seq_t)6;    /* a(n-2) */
    seq_t a2 = (seq_t)4;    /* a(n-1) */
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i) {
        ai = (seq_t)2 * a2 + (seq_t)2 * a1 - a0;  /* recurrence */
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}

/*
 * A379825  a(n) = [x^n] x/(12*x^2 - 6*x + 1).
 * https://oeis.org/A379825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=0, a(1)=1, a(n) = 6*a(n-1) - 12*a(n-2) for n >= 2
 *            (second‑order linear recurrence with constant coefficients)
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 18, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t prev2 = 0; /* a(n-2) */
    seq_t prev1 = 1; /* a(n-1) */
    seq_t cur   = 0; /* a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        cur = 6 * prev1 - 12 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}

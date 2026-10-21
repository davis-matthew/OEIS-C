/*
 * A296817  Expansion of 1/Sum_{k>=0} (2*k+1)^2*x^k.
 * https://oeis.org/A296817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -6*a(n-1) - a(n-2) a(0) = 1, a(1) = -9, a(2) = 56, a(3) =
 *            -328
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 11, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-9;
    if (n == 2) return (seq_t)56;
    if (n == 3) return (seq_t)-328;

    /* Use two-move rolling recurrence to avoid any memory or
       indexing complications. */
    seq_t prev2 = (seq_t)56;      /* a(2) */
    seq_t prev1 = (seq_t)-328;    /* a(3) */
    seq_t cur   = 0;              /* placeholder for a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        /* cur = -6*prev1 - prev2 */
        seq_t temp = (seq_t)6 * prev1;
        cur = -temp - prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;   /* cur holds a(n) after the loop finishes */
}

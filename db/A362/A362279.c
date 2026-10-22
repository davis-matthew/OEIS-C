/*
 * A362279  Expansion of e.g.f. exp(x - 5*x^2/2).
 * https://oeis.org/A362279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1, a(1) = 1 a(n) = a(n-1) - 5*(n-1)*a(n-2) for n > 1 The
 *            function computes a(n) using this recurrence, evaluating
 *            sequentially from 0 up to n (O(n) time, O(1) memory)
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 14, i64 n <= 23, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)1;   /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t mult = (seq_t)5 * ((seq_t)(i - 1));
        curr = prev1 - mult * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

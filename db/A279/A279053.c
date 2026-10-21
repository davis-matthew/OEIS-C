/*
 * A279053  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 131", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A279053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 100*a(n-2) - 100*a(n-3); Initial values: a(0) =
 *            1, a(1) = 1, a(2) = 101. This recurrence holds for all n > 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)101;

    seq_t prev3 = (seq_t)1;   /* a(0) */
    seq_t prev2 = (seq_t)1;   /* a(1) */
    seq_t prev1 = (seq_t)101; /* a(2) */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t cur = prev1 + (seq_t)100 * prev2 - (seq_t)100 * prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}

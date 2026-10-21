/*
 * A282088  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 553", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A282088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 101*a(n-2) - 100*a(n-4) for n > 4; base values: a(0) = 1,
 *            a(2) = 1, a(4) = 101; a(odd) = 0
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* odd positions are zero */
    if ((n & 1) != 0)
        return (seq_t)0;

    /* known base cases */
    if (n == 0)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)1;
    if (n == 4)
        return (seq_t)101;

    /* recurrence for even positions (n >= 6) */
    seq_t prev4 = (seq_t)1;   /* a(2)  */
    seq_t prev2 = (seq_t)101; /* a(4)  */
    seq_t curr;

    for (idx_t k = 6; k <= n; k += 2) {
        curr = (seq_t)101 * prev2 - (seq_t)100 * prev4;
        prev4 = prev2;
        prev2 = curr;
    }
    return prev2;
}

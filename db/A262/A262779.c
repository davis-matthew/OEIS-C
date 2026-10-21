/*
 * A262779  Binary representation of the n-th iteration of the "Rule 175"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A262779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 110, a(2) = 11011, a(n) = 101*a(n-1)-100*a(n-2)
 *            for n > 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)110;
    if (n == 2) return (seq_t)11011;

    /* Use recurrence: a(n) = 101*a(n-1) - 100*a(n-2)  (n > 2) */
    seq_t prev2 = (seq_t)110;   /* a(n-2) for n = 3 */
    seq_t prev1 = (seq_t)11011; /* a(n-1) for n = 3 */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t cur = ((seq_t)101 * prev1) - ((seq_t)100 * prev2);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

/*
 * A282579  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 553", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A282579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for odd n; a(0)=1, a(2)=4, a(4)=20; for even n≥6, a(n)
 *            = 5*a(n-2) - 4*a(n-4)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n & 1ULL) return (seq_t)0;   /* odd n */

    /* Even n */
    if (n == 2) return (seq_t)4;
    if (n == 4) return (seq_t)20;

    /* Recurrence for n ≥ 6, stepping by 2 */
    seq_t prev2 = (seq_t)4;   /* a(2) */
    seq_t prev1 = (seq_t)20;  /* a(4) */
    seq_t cur   = (seq_t)0;

    for (idx_t k = 6; k <= n; k += 2) {
        cur   = (seq_t)5 * prev1 - (seq_t)4 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}

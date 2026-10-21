/*
 * A284352  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 899", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A284352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 100*a(n-2) - 100*a(n-3) a(0) = 1, a(1) = 10,
 *            a(2) = 111 (valid for n ≥ 3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)111;

    seq_t prev3 = (seq_t)1;   /* a(0) */
    seq_t prev2 = (seq_t)10;  /* a(1) */
    seq_t prev1 = (seq_t)111; /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = prev1 + ((seq_t)100) * prev2 - ((seq_t)100) * prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}

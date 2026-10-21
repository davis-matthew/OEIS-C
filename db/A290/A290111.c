/*
 * A290111  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 643", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^(n+1)-91)/9 for n>=2, a(0)=1, a(1)=11. Equivalent
 *            recurrence: a(n)=11*a(n-1)-10*a(n-2) for n>=4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (n == 1)
        return 11;
    if (n == 2)
        return 101;
    if (n == 3)
        return 1101;

    seq_t prev2 = 101;     /* a(2) */
    seq_t prev1 = 1101;    /* a(3) */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i) {
        cur = prev1 * (seq_t)11 - prev2 * (seq_t)10;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

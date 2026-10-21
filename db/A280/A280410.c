/*
 * A280410  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 261", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A280410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n even, a(n) = 10*(10^n - 1)/9 for n odd.
 *            Recurrence: a(0)=1, a(1)=10, a(2)=1, a(3)=1110, a(n) =
 *            101*a(n-2) - 100*a(n-4) for n>3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* even indices are always 1 */
    if ((n & 1u) == 0)
        return 1;

    /* handle the two odd base cases */
    if (n == 1)
        return 10;
    if (n == 3)
        return 1110;

    /* iteratively apply the recurrence for odd n>=5 */
    seq_t prev4 = 10;    /* a(1) */
    seq_t prev2 = 1110;  /* a(3) */
    seq_t cur;
    for (idx_t i = 5; i <= n; i += 2) {
        cur = prev2 * 101u - prev4 * 100u;
        prev4 = prev2;
        prev2 = cur;
    }
    return prev2;
}

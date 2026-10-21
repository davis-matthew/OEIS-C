/*
 * A277560  Binary representation of the x-axis, from the left edge to the
 *          origin, or from the origin to the right edge, of the n-th stage of
 *          growth of the two-dimensional cellular automaton defined by "Rule
 *          7", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A277560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = 0 if n is even and n > 0; a(n) = (10^(n+1) - 1)
 *            / 9 if n is odd (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* the initial single black cell */
        return 1;

    if ((n & 1U) == 0)   /* even n (except n==0) produces 0 */
        return 0;

    /* odd n: construct the number consisting of (n+1) ones.
       Do it by repeated multiplication by 10 and addition of 1. */
    seq_t v = 0;
    for (idx_t i = 0; i <= n; ++i)
        v = v * 10 + 1;

    return v;
}

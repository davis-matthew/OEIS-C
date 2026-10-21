/*
 * A277926  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 5", based on the 5-celled von
 *          Neumann neighborhood.
 * https://oeis.org/A277926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(1) = 10 a(n) = 0 for n > 1 even a(n) = (10^(n+1)-1)/9
 *            for n > 1 odd (Recurrence a(n) = 101*a(n-2) - 100*a(n-4) for n >
 *            5 is not used)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)10;
    if ((n & (idx_t)1) == 0)
        return (seq_t)0;

    /* n is odd, n >= 3 */
    seq_t val = 0;
    idx_t digits = n + 1;
    for (idx_t i = 0; i < digits; ++i)
        val = val * 10 + 1;
    return val;
}

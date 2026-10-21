/*
 * A282797  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 505", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A282797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n) = (10^(n+1) - 1) / 9 for n even a(n) = (10^n - 10) /
 *            99 for n odd
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t p = 1;
    if ((n & 1ULL) == 0) {          /* n even: 10^(n+1) */
        for (idx_t i = 0; i <= n; ++i)
            p *= 10;
        return (p - 1) / 9;
    } else {                        /* n odd: 10^n      */
        for (idx_t i = 0; i < n; ++i)
            p *= 10;
        return (p - 10) / 99;
    }
}

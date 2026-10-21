/*
 * A283508  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 643", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A283508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(1) = 10; a(n) = a(n-1) * 10 + 1 for n >= 2 (OEIS
 *            A283508)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;
    if (n == 1u)
        return (seq_t)10;

    seq_t val = (seq_t)10;   /* a(1) */
    for (idx_t i = 2u; i <= n; ++i) {
        val *= (seq_t)10;
        val += (seq_t)1;
    }
    return val;
}

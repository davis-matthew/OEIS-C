/*
 * A278758  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 73", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A278758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n = 0, * 0 if n is even or n = 1, * 110 if n = 2, *
 *            101 if n = 3, * for odd n ≥ 5: a(n) = decimal number consisting
 *            of (n + 1) digits all equal to 1. * The latter can be computed
 *            as a(n) = 1 + 10 + 10² + … + 10ⁿ
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Fixed initial terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)110;
    if (n == 3) return (seq_t)101;

    /* Even indices produce 0 */
    if ((n & 1ULL) == 0) return (seq_t)0;

    /* Odd indices ≥ 5: build the decimal string of all ones */
    idx_t len = n + 1;          /* number of digits */
    seq_t val = 0;
    for (idx_t i = 0; i < len; ++i) {
        val = val * 10 + 1;
    }
    return val;
}

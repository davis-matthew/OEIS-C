/*
 * A267353  Total number of ON (black) cells after n iterations of the "Rule
 *          123" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A267353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n + 1)*(n + 2)/2 if n is even, n*(n + 5)/2 if n is odd
 * Range:     u8 n <= 20, u16 n <= 360, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n & 1) == 0) {
        /* n even: (n+1)(n+2)/2 */
        return ((nn + 1) * (nn + 2)) / (seq_t)2;
    } else {
        /* n odd: n(n+5)/2 */
        return (nn * (nn + 5)) / (seq_t)2;
    }
}

/*
 * A266438  Total number of ON (black) cells after n iterations of the "Rule
 *          23" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A266438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2+2*n-(-1)^n*(n+1)+3)/2
 * Range:     u8 n <= 22, u16 n <= 360, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    if ((n & 1) == 0) {          /* n is even: a(n) = (n^2 + n + 2)/2 */
        return (nn * nn + nn + 2) / 2;
    } else {                     /* n is odd : a(n) = (n^2 + 3*n + 4)/2 */
        return (nn * nn + 3 * nn + 4) / 2;
    }
}

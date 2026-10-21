/*
 * A265724  Total number of OFF (white) cells after n iterations of the "Rule
 *          1" elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A265724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/2*(n^2 + (-1)^n*n + 4*n - (-1)^n + 1)
 * Range:     u8 n <= 15, u16 n <= 272, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    if ((n & 1ULL) == 0) {           /* n even: a(n) = (n^2 + 5*n)/2 */
        return (n2 + nn * 5) / 2;
    } else {                         /* n odd:  a(n) = (n^2 + 3*n + 2)/2 */
        return (n2 + nn * 3 + 2) / 2;
    }
}

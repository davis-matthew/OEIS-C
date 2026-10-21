/*
 * A266259  Total number of OFF (white) cells after n iterations of the "Rule
 *          11" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A266259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+1)^2+(-1)^n*(n-1))/2
 * Range:     u8 n <= 15, u16 n <= 352, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t m = (seq_t)(n + 1);          /* n+1 */
    seq_t square = m * m;              /* (n+1)^2 */
    seq_t term = (seq_t)(n - 1);        /* n-1 (n>=1) */

    seq_t numerator;
    if (n & 1u)                        /* odd n: subtract term */
        numerator = square - term;
    else                               /* even n: add term */
        numerator = square + term;

    return numerator / 2;
}

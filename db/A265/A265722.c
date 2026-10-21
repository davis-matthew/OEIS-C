/*
 * A265722  Number of ON (black) cells in the n-th iteration of the "Rule 1"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A265722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is even, a(n) = 2*n - 2 if n is odd. Equivalent
 *            closed form from OEIS: a(n) = 1/2*(-2*(-1)^n*n + 2*n + 3*(-1)^n
 *            - 1)
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* Convert index to type used for calculation. */
    if ((nn & 1) == 0)            /* even n */
        return (seq_t)1;
    else                           /* odd n */
        return (seq_t)(2 * nn - 2);
}

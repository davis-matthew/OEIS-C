/*
 * A386226  The maximum possible number of 8-cycles in an outerplanar graph on
 *          n vertices.
 * https://oeis.org/A386226
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*n - 99 + 5*floor(n/4) - 3*floor((n+3)/4) for n ≥ 12
 *            (and the explicit values for 8 ≤ n < 12). For n < 8 the function
 *            returns 0. This implements the OEIS sequence A386226
 * Range:     u8 n <= 34, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 8)
        return 0;          /* undefined for n < 8, but works for required n ≥ 8 */

    /* explicit values for 8 ≤ n < 12 */
    if (n == 8) return 1;
    if (n == 9) return 4;
    if (n == 10) return 10;
    if (n == 11) return 16;

    /* n ≥ 12: formula */
    seq_t nn = (seq_t)n;
    /* floor divisions are natural integer divisions for unsigned types */
    seq_t term = 10 * nn - 99
                 + 5 * (nn / 4)
                 - 3 * ((nn + 3) / 4);
    return term;
}

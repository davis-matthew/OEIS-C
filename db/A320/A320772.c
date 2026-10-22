/*
 * A320772  Prime generating polynomial: a(n) = (4*n - 29)^2 + 58.
 * https://oeis.org/A320772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n - 29)^2 + 58 (offset 1)
 * Range:     u8 none, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ensure n >= 1; behaviour for n == 0 is undefined but return 0 */
    if (n == 0)
        return 0;

    seq_t nn = (seq_t)n;           /* convert index to seq_t */
    seq_t t  = (seq_t)4 * nn - 29; /* compute 4*n - 29 */
    return t * t + 58;             /* square and add 58 */
}

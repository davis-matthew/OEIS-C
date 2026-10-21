/*
 * A269439  Number of length-7 0..n arrays with no repeated value greater than
 *          the previous repeated value.
 * https://oeis.org/A269439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^7 + 7*n^6 + 16*n^5 + (71/3)*n^4 + (139/6)*n^3 +
 *            (43/3)*n^2 + (35/6)*n + 1. The expression is evaluated as whole
 *            + (fraction/6) to avoid intermediate fractions
 * Range:     u8 none, u16 n <= 3, u32 n <= 22, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for multiplication */
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;          /* n^2 */
    seq_t n3 = n2 * sn;          /* n^3 */
    seq_t n4 = n3 * sn;          /* n^4 */
    seq_t n5 = n4 * sn;          /* n^5 */
    seq_t n6 = n5 * sn;          /* n^6 */
    seq_t n7 = n6 * sn;          /* n^7 */

    /* Integer part of the polynomial */
    seq_t whole = n7 + (seq_t)7 * n6 + (seq_t)16 * n5 + (seq_t)1;

    /* Numerator of the fractional part (common denominator 6) */
    seq_t frac_num = (seq_t)142 * n4
                   + (seq_t)139 * n3
                   + (seq_t)86  * n2
                   + (seq_t)35  * sn;

    /* Combine integer and fractional parts */
    seq_t result = whole + (frac_num / (seq_t)6);
    return result;
}

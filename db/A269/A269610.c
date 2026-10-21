/*
 * A269610  Number of length-7 0..n arrays with no repeated value differing
 *          from the previous repeated value by one or less.
 * https://oeis.org/A269610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^7 + 7*n^6 + 16*n^5 -12*n^4 -5*n^3 +18*n^2 -15*n +4
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 22, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t res = 1;          /* coefficient of n^7 */

    res = res * tn + 7;     /* add coefficient of n^6 */
    res = res * tn + 16;    /* add coefficient of n^5 */
    res = res * tn;
    res -= 12;              /* subtract coefficient of n^4 */
    res = res * tn;
    res -= 5;               /* subtract coefficient of n^3 */
    res = res * tn + 18;    /* add coefficient of n^2 */
    res = res * tn;
    res -= 15;              /* subtract coefficient of n   */
    res = res * tn + 4;    /* add constant term */

    return res;
}

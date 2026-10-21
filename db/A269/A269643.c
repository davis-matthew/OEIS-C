/*
 * A269643  Number of length-6 0..n arrays with no repeated value differing
 *          from the previous repeated value by other than plus two or minus
 *          1.
 * https://oeis.org/A269643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^6 + 6*n^5 + 5*n^4 + 12*n^3 − 12*n^2 + 9*n − 7 for n>2
 *            a(1) = 16, a(2) = 396
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 39, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)396;
    /* Use Horner's rule to evaluate the degree‑6 polynomial efficiently. */
    seq_t tn = (seq_t)n;
    seq_t res = (seq_t)1;          /* coefficient of n^6 */
    res = res * tn + 6;            /* +6*n^5 */
    res = res * tn + 5;            /* +5*n^4 */
    res = res * tn + 12;           /* +12*n^3 */
    res = res * tn - 12;           /* −12*n^2 */
    res = res * tn + 9;            /* +9*n */
    res = res * tn - 7;            /* −7 */
    return res;
}

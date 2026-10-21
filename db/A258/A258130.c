/*
 * A258130  Octagonal numbers (A000567) that are the sum of ten consecutive
 *          octagonal numbers.
 * https://oeis.org/A258130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1443*a(n-1) - 1443*a(n-2) + a(n-3), with a(1) = 1045,
 *            a(2) = 1325345, a(3) = 1910970885
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 1) return (seq_t)1045;
    if (n == 2) return (seq_t)1325345;
    if (n == 3) return (seq_t)1910970885;

    /* Iterative calculation for n >= 4 */
    seq_t prev3 = (seq_t)1045;          /* a(n-3)   */
    seq_t prev2 = (seq_t)1325345;       /* a(n-2)   */
    seq_t prev1 = (seq_t)1910970885;    /* a(n-1)   */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i) {
        cur = (seq_t)1443 * prev1 - (seq_t)1443 * prev2 + prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

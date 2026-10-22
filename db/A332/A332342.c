/*
 * A332342  Table T(n, k) read by antidiagonals upwards: sum of the terms of
 *          the continued fraction for the fractional part of n/k (n>=1,
 *          k>=1).
 * https://oeis.org/A332342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum of the partial quotients of the simple continued
 *            fraction of the fractional part of i/k, where (i,k) is the pair
 *            that appears at the n‑th position when the infinite table T(i,k)
 *            = sum of the terms of the continued fraction of fractional part
 *            of (i/k) is read along the upward antidiagonals
 * Range:     u8 n <= 4950, u16 n <= 4950, u32 n <= 4950, u64 n <= 4950,
 *            u128 n <= 4950, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* find the diagonal sum s such that s*(s-1)/2 >= n */
    double d = sqrt((double)n * 8.0 + 1.0);
    idx_t s = (idx_t)((1.0 + d) / 2.0);       /* initial guess */

    /* correct possible rounding errors */
    while ( (s-1) * s / 2 < n ) s++;          /* ensure enough elements */
    while ( (s-2) * (s-1) / 2 >= n ) s--;     /* keep smallest s     */

    /* the number of elements before this diagonal */
    idx_t before = (s-1) * (s-2) / 2;

    /* offset within the diagonal (0-based) */
    idx_t d_off = n - before - 1;             /* d_off in [0, s-2]   */

    /* coordinates of the table entry */
    idx_t i = s-1 - d_off;                    /* row (n)  */
    idx_t k = s - i;                          /* column     */

    /* compute the sum of continued‑fraction terms of the fractional part of i/k */
    seq_t sum = 0;
    seq_t a_num = (seq_t)(i % k);             /* numerator of fractional part */
    seq_t b_den = (seq_t)k;                   /* denominator            */

    while (a_num != 0) {
        seq_t q = b_den / a_num;
        sum += q;
        seq_t r = b_den % a_num;
        b_den = a_num;
        a_num = r;
    }

    return sum;
}

/*
 * A303120  Total area of all rectangles of size p X q such that p + q = n^2
 *          and p <= q.
 * https://oeis.org/A303120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n^2/2)} i * (n^2 - i)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 60, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n^2 as seq_t */
    seq_t n_sq = (seq_t)n * (seq_t)n;

    /* m = floor(n^2 / 2) */
    seq_t m = n_sq / 2;

    /* sum_i = m*(m+1)/2 */
    seq_t sum_i = m * (m + 1);
    sum_i /= 2;

    /* sum_i2 = m*(m+1)*(2*m+1)/6 */
    seq_t sum_i2 = m * (m + 1);
    sum_i2 *= (2 * m + 1);
    sum_i2 /= 6;

    /* result = n^2 * sum_i - sum_i2 */
    seq_t result = n_sq * sum_i;
    result -= sum_i2;

    return result;
}

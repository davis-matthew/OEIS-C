/*
 * A294300  Sum of the fifth powers of the parts in the partitions of n into
 *          two distinct parts.
 * https://oeis.org/A294300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..(n-1)/2} (i^5 + (n-i)^5)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 54, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t m = (n - 1) / 2;          /* floor((n-1)/2) */

    for (idx_t i = 1; i <= m; ++i) {
        /* compute i^5 */
        seq_t ii = (seq_t)i;
        seq_t i2 = ii * ii;          /* i^2 */
        seq_t i3 = i2 * ii;          /* i^3 */
        seq_t i4 = i3 * ii;          /* i^4 */
        seq_t i5 = i4 * ii;          /* i^5 */

        /* compute (n-i)^5 */
        seq_t ni = (seq_t)(n - i);
        seq_t ni2 = ni * ni;
        seq_t ni3 = ni2 * ni;
        seq_t ni4 = ni3 * ni;
        seq_t ni5 = ni4 * ni;

        result += i5 + ni5;
    }
    return result;
}

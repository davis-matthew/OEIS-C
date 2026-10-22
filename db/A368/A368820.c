/*
 * A368820  Sum of the squares less than n that do not divide n.
 * https://oeis.org/A368820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of the squares s < n such that s ∤ n
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A368820 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n <= 1) return sum;          /* n = 1 -> no squares < 1 */

    for (idx_t m = 1; (m * m) < n; ++m) {
        idx_t sq = m * m;            /* sq < n, fits in idx_t */
        if (n % sq != 0) {
            sum += (seq_t)sq;
        }
    }
    return sum;
}

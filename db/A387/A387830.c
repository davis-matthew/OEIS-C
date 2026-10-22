/*
 * A387830  Least number of vertical or horizontal cuts required to divide a
 *          square into n equal parts.
 * https://oeis.org/A387830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min_{d|n} (d-1)+(n/d-1) (i.e. min_{d|n} d + n/d - 2)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base case for n = 1 */
    if (n <= 1)
        return (seq_t)0;

    /* Candidate with d = 1 gives a(n) = n-1 */
    seq_t min = (seq_t)(n - 1);

    /* Iterate over divisors d <= sqrt(n) */
    for (idx_t d = 2; d <= n / d; ++d) {
        if (n % d == 0) {
            seq_t val = (seq_t)(d - 1) + (seq_t)(n / d - 1);
            if (val < min)
                min = val;
        }
    }
    return min;
}

/*
 * A363316  Sum of divisors of 4*n-2 of form 4*k+1.
 * https://oeis.org/A363316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d| (4*n-2), d mod 4 == 1} d
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* compute m = 4*n - 2 */
    idx_t m = (idx_t)4 * n - (idx_t)2;
    seq_t sum = 0;
    /* iterate over divisors up to sqrt(m) */
    for (idx_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            if (i % 4 == 1) sum += (seq_t)i;
            idx_t j = m / i;
            if (j != i && j % 4 == 1) sum += (seq_t)j;
        }
    }
    return sum;
}

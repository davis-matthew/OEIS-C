/*
 * A274450  Largest number of antipower periods possible for a binary string
 *          of length n.
 * https://oeis.org/A274450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n} [n/d <= 2^d]
 * Range:     u8 n <= 32, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for zero, not part of the sequence */

    uint64_t count = 0;
    for (idx_t d = 1; d <= n / d; ++d) {          /* iterate divisors up to sqrt(n) */
        if (n % d == 0) {
            /* divisor d */
            uint64_t d1 = d;
            uint64_t n_over_d1 = (uint64_t)(n / d1);

            bool cond1 = (d1 >= 64) ? true
                : (n_over_d1 <= (1ULL << d1));
            if (cond1) ++count;

            /* paired divisor n/d1 */
            uint64_t d2 = n / d1;
            if (d2 != d1) {
                uint64_t n_over_d2 = d1;           /* since n/d2 == d1 */
                bool cond2 = (d2 >= 64) ? true
                    : (n_over_d2 <= (1ULL << d2));
                if (cond2) ++count;
            }
        }
    }

    return (seq_t)count;
}

/*
 * A359241  Number of divisors of 5*n-4 of form 5*k+4.
 * https://oeis.org/A359241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A001899(5*n-4)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* offset: a(1) is the first term */
        return 0;

    /* compute m = 5*n-4 safely in 64‑bit arithmetic */
    uint64_t m = (uint64_t)5 * n - 4;

    seq_t count = 0;
    for (uint64_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            if (d % 5 == 4)
                ++count;
            uint64_t d2 = m / d;
            if (d2 != d && d2 % 5 == 4)
                ++count;
        }
    }
    return count;
}

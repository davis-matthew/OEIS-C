/*
 * A363858  Number of divisors of 7*n-6 of form 7*k+6.
 * https://oeis.org/A363858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of (7*n - 6) that are congruent to 6
 *            (mod 7)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* m = 7*n - 6, computed in 64‑bit to avoid overflow of seq_t */
    uint64_t m = 7ULL * (uint64_t)n - 6ULL;

    uint64_t count = 0;
    for (uint64_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            uint64_t d1 = d;
            uint64_t d2 = m / d;

            if (d1 % 7ULL == 6ULL) ++count;
            if (d2 != d1 && d2 % 7ULL == 6ULL) ++count;
        }
    }
    return (seq_t)count;
}

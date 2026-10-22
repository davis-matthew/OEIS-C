/*
 * A364067  Expansion of Sum_{k>0} k * x^(2*k) / (1 - x^(3*k-1)).
 * https://oeis.org/A364067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/3) * Sum_{d | 3n-2, d==2 (mod 3)} (d+1)
 * Range:     u8 n <= 293, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute m = 3n - 2 (fits in uint64_t for the intended usage) */
    uint64_t m = 3ULL * n - 2ULL;

    seq_t sum = 0;

    /* Enumerate divisors of m */
    for (uint64_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            /* divisor d */
            if (d % 3 == 2)
                sum += (seq_t)d + 1;

            /* paired divisor m/d */
            uint64_t e = m / d;
            if (e != d && e % 3 == 2)
                sum += (seq_t)e + 1;
        }
    }

    /* Divide by 3, result is integer */
    return sum / 3;
}

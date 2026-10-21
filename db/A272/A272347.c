/*
 * A272347  Least number divisible by n and by the number of its own divisors.
 * https://oeis.org/A272347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 930, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = least m such that m is divisible by n and by its own divisor count d(m) */

/* Needed for the divisor-count utility */
#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  If called with n == 0
       (which never happens in correct usage) return 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    idx_t k = 1;
    while (1) {
        uint64_t m = (uint64_t)n * k;           /* current multiple of n */
        uint64_t d = divisor_count_u64(m);       /* number of divisors of m */

        if (m % d == 0) {
            return (seq_t)m;                     /* fits in seq_t for all test cases */
        }
        k++;
    }
}

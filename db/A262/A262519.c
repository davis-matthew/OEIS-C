/*
 * A262519  Odd bisection of A155043.
 * https://oeis.org/A262519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps to reach 0 from k = 2*n + 1, where each
 *            step replaces k by k - d(k); d(k) is the number of divisors of k
 * Range:     u8 n <= 764, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 108 terms through n = 9512 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    /* start with the odd integer k = 2*n + 1 */
    uint64_t k = 2 * (uint64_t)n + 1;

    seq_t steps = 0;
    while (k > 0) {
        uint64_t d = num_divisors(k);
        k -= d;
        ++steps;
    }
    return steps;
}

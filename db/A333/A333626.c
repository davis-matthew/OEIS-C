/*
 * A333626  Number of ways to write n as the sum of two positive integers that
 *          have the same number of divisors.
 * https://oeis.org/A333626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} [d(i) = d(n-i)]
 * Range:     u8 n <= 1259, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 99 terms through n = 9550 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where d(m) is the divisor counting function.  */

#include <stdint.h>
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* defined only for n ≥ 1 */

    seq_t res = 0;
    idx_t half = n / 2;
    for (idx_t i = 1; i <= half; ++i) {
        seq_t di = num_divisors((uint64_t)i);
        seq_t dj = num_divisors((uint64_t)(n - i));
        if (di == dj)
            res += 1;
    }
    return res;
}

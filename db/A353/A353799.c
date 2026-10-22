/*
 * A353799  a(n) = 1 if the number of divisors of n is not larger than 4,
 *          otherwise 0.
 * https://oeis.org/A353799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if d(n) <= 4 else 0, where d(n) is the divisor count
 * Range:     u8 n <= 9732, u16 n <= 9732, u32 n <= 9732, u64 n <= 9732,
 *            u128 n <= 9732, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence counts whether the number of divisors of n is at most 4.
   We factor n (using the provided factor_u64 helper), compute the
   product of (exponent+1) for each prime factor, and test if it
   is <=4. For n = 1 the divisor count is 1. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static inline seq_t _divisor_count_u128(uint64_t n)
{
    if (n == 1) return 1;   /* only divisor is 1 */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(n, primes, exps, &cnt);

    seq_t d = 1;
    for (size_t i = 0; i < cnt; ++i) {
        d *= (seq_t)(exps[i] + 1);
        if (d > 4) break;   /* early exit */
    }
    return d;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* outside of offset, defined as 0 */
    if (n == 1) return 1;                /* 1 has 1 divisor */

    seq_t d = _divisor_count_u128((uint64_t)n);
    return (seq_t)(d <= 4 ? 1 : 0);
}

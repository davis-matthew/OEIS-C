/*
 * A280583  a(n) = product of divisors of the number of divisors of n.
 * https://oeis.org/A280583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of divisors of τ(n) where τ(n) is the number of
 *            divisors of n
 * Range:     u8 n <= 59, u16 n <= 359, u32 n <= 5039, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* τ(n) */
    uint64_t tau_n = divisor_count_u64((uint64_t)n);

    /* product of all divisors of τ(n) */
    seq_t prod = 1;
    for (uint64_t d = 1; d * d <= tau_n; ++d)
    {
        if (tau_n % d == 0)
        {
            prod *= (seq_t)d;
            uint64_t e = tau_n / d;
            if (e != d)
                prod *= (seq_t)e;
        }
    }
    return prod;
}

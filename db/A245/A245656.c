/*
 * A245656  Characteristic function of arithmetic numbers, cf. A003601.
 * https://oeis.org/A245656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if sigma(n) % tau(n) == 0, else 0 where sigma(n) = sum
 *            of divisors of n, tau(n) = number of divisors of n
 * Range:     u8 n <= 6955, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9790 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/num_divisors.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n) {
    /* The OEIS offset starts at n = 1. For n < 1 we simply return 0. */
    if (n < 1) return 0;

    seq_t sigma = sigma_of(n);                 /* sum of divisors */
    uint64_t tau_u64 = num_divisors(n);   /* divisor count (uint64_t) */
    seq_t tau = (seq_t) tau_u64;               /* cast to seq_t for modulo */

    return (sigma % tau == 0) ? (seq_t)1 : (seq_t)0;
}

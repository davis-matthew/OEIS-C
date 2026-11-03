/*
 * A296084  a(1) = 0 and for n > 1, a(n) = 1 if tau(n)-1 divides sigma(n)-1, 0
 *          otherwise. Here tau = A000005, sigma = A000203.
 * https://oeis.org/A296084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if τ(n)-1 divides σ(n)-1, 0 otherwise
 * Range:     u8 n <= 3866, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 136 terms through n = 8424 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* τ(n) */
    seq_t tau = num_divisors((uint64_t)n);

    /* σ(n) */
    seq_t sigma = sigma_of(n);

    seq_t tau_minus = tau - 1;
    seq_t sigma_minus = sigma - 1;

    return (sigma_minus % tau_minus == 0) ? (seq_t)1 : (seq_t)0;
}

/*
 * A296084  a(1) = 0 and for n > 1, a(n) = 1 if tau(n)-1 divides sigma(n)-1, 0
 *          otherwise. Here tau = A000005, sigma = A000203.
 * https://oeis.org/A296084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if τ(n)-1 divides σ(n)-1, 0 otherwise
 * Range:     u8 n <= 3866, u16 n <= 8424, u32 n <= 8424, u64 n <= 8424,
 *            u128 n <= 8424, big any n
 * Helpers:   divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* τ(n) */
    seq_t tau = divisor_count_u64((uint64_t)n);

    /* σ(n) */
    seq_t sigma = sigma_of(n);

    seq_t tau_minus = tau - 1;
    seq_t sigma_minus = sigma - 1;

    return (sigma_minus % tau_minus == 0) ? (seq_t)1 : (seq_t)0;
}

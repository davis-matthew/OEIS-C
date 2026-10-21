/*
 * A279507  a(n) = floor(phi(n)/tau(n)).
 * https://oeis.org/A279507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(phi(n) / tau(n)) where tau is the divisor count
 * Range:     u8 n <= 272, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   factor_u64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t and idx_t are provided by the build.  The helper functions
 * from the utils library are used, specifically phi and factor_u64.
 * A local helper computes the divisor count for uint64_t n.
 */
#include <stdint.h>
#include <stddef.h>
#include "../utils/phi.h"
#include "../utils/factor_u64.h"

/* compute the number of divisors tau(n) for a 64‑bit integer n */
static seq_t tau_u64(uint64_t n)
{
    if (n <= 1U)
        return (seq_t)1;   /* tau(1) = 1 */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    seq_t tau = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        tau = tau * (seq_t)(exps[i] + 1);
    }
    return tau;
}

seq_t a(idx_t n)
{
    /* phi from utils returns seq_t */
    seq_t phi_val = phi(n);

    /* tau is computed as seq_t; it fits in seq_t for all n used in tests */
    seq_t tau_val = tau_u64((uint64_t)n);

    /* floor of the division; integer division is truncating toward 0 */
    return phi_val / tau_val;
}

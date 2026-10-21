/*
 * A280685  a(n) = sum of the divisors of the product of the divisors of n.
 * https://oeis.org/A280685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = σ( Π_{d|n} d ) = σ( n^{τ(n)/2} )
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 59, u64 n <= 179,
 *            u128 n <= 719, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where σ is sum-of-divisors, τ(n) is divisor count. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* no defined value for n=0; return 0 */

    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* compute τ(n) */
    seq_t tau = 1;
    for (size_t i = 0; i < cnt; ++i)
        tau = tau * ((seq_t)exps[i] + 1);

    /* compute σ(n^{τ/2}) = ∏ σ(p_i^{new_exp}) with new_exp = e_i * τ / 2 */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t e = (seq_t)exps[i];
        seq_t temp = e * tau;                /* e * τ  */
        seq_t new_exp = temp / 2;           /* floor( e*τ/2 )  */
        idx_t exp_plus_one = (idx_t)(new_exp + 1); /* exponent for numerator */
        seq_t numerator = seq_pow((seq_t)primes[i], exp_plus_one) - 1;
        seq_t denom = (seq_t)primes[i] - 1;
        seq_t term = numerator / denom;
        result = result * term;
    }

    return result;
}

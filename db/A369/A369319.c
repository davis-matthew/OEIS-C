/*
 * A369319  a(n) is the sum of the greatest common exponential divisor of n
 *          and k over the positive numbers k that do not exceed n.
 * https://oeis.org/A369319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, phi, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = product_{p^e||n} ( sum_{d|e} p^d * φ(e/d) )
 *
 * The function is multiplicative: each prime power contributes
 * a(p^e) = Σ_{k=1..e} p^{gcd(e,k)} = Σ_{d|e} p^d·φ(e/d).
 * The product over all prime powers gives the value for n.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"
#include "../utils/phi.h"

/* Compute a(p^e) for a prime power: Σ_{d|e} p^d·φ(e/d) */
static seq_t a_prime_power(uint64_t p, uint64_t e)
{
    seq_t sum = 0;
    for (uint64_t d = 1; d <= e; ++d) {
        if (e % d == 0) {
            seq_t pow_val = pow_uint(p, d);        /* p^d */
            seq_t phi_val = phi((idx_t)(e / d));   /* φ(e/d) */
            sum += pow_val * phi_val;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    seq_t result = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        result *= a_prime_power(primes[i], exps[i]);
    }
    return result;
}

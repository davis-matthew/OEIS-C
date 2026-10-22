/*
 * A324892  Multiplicative with a(p^e) = p^e if sigma(p^e) is prime, and 1
 *          otherwise.
 * https://oeis.org/A324892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n, σ(p^e) prime} p^e (Multiplicative; σ(p^e)=
 *            (p^(e+1)-1)/(p-1))
 * Range:     u8 n <= 16, u16 n <= 9696, u32 n <= 9696, u64 n <= 9696,
 *            u128 n <= 9696, big any n
 * Helpers:   factor_u64, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1; n==0 is undefined, return 0 */
        return (seq_t)0;

    /* Factor n; primes and exponents are stored in arrays. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p     = (seq_t)primes[i];   /* the prime itself as seq_t */
        uint64_t e  = exps[i];           /* exponent in the factorisation */

        /* Compute p^e. */
        seq_t p_pow = 1;
        for (uint64_t k = 0; k < e; ++k)
            p_pow *= p;

        /* Compute σ(p^e) = (p^(e+1) - 1) / (p - 1). */
        seq_t p_pow_next = p_pow * p;              /* p^(e+1) */
        seq_t numerator   = p_pow_next - 1;        /* p^(e+1) - 1 */
        seq_t denominator = p - 1;                 /* p - 1 */
        seq_t sigma       = numerator / denominator;

        /* Include this prime power in the product only if σ(p^e) is prime. */
        if (is_prime_big(sigma))
            result *= p_pow;
    }

    return result;
}

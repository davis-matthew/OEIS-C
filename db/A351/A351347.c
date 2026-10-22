/*
 * A351347  Dirichlet g.f.: Product_{p prime} 1 / (1 - p^(-s) - 2*p^(-2*s)).
 * https://oeis.org/A351347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} J_{e+1} where J_k = (2^k - (-1)^k)/3
 *            (Jacobsthal numbers)
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t count = 0;

    /* factorize n; factor_u64 fills primes[] and exps[] with prime factors
       and their exponents and sets count to the number of distinct primes. */
    factor_u64(n, primes, exps, &count);

    seq_t result = 1;

    for (size_t i = 0; i < count; ++i)
    {
        uint64_t e = exps[i];          /* exponent of this prime factor */
        idx_t k = e + 1;               /* k = e+1 for Jacobsthal */
        seq_t pow = pow2(k);           /* 2^k */

        /* numerator: 2^k - (-1)^k = 2^k + 1 if k odd, 2^k - 1 if k even */
        seq_t numerator = (k & 1) ? (pow + 1) : (pow - 1);
        seq_t J = numerator / 3;       /* Jacobsthal number J_{k} */

        result = result * J;
    }

    return result;
}

/*
 * A353753  a(n) = phi(sigma(n)) - Product_{p^e||n} phi(sigma(p^e)), where n =
 *          Product_{p^e||n}, with each p^e the maximal power of prime p that
 *          divides n.
 * https://oeis.org/A353753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(sigma(n)) – Π_{p^e||n} phi(sigma(p^e))
 * Range:     u8 n <= 16, u16 n <= 1202, u32 n <= 9137, u64 n <= 9137,
 *            u128 n <= 9137, big any n
 * Helpers:   calc_phi, factor_u64, pow_uint, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                           where sigma is the sum-of-divisors function, phi is Euler's totient. */

#include <stdint.h>
#include "../utils/sigma_of.h"      /* seq_t sigma_of(idx_t n);                 */
#include "../utils/calc_phi.h"      /* seq_t calc_phi(seq_t m);                 */
#include "../utils/factor_u64.h"    /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/pow_uint.h"      /* seq_t pow_uint(uint64_t base, idx_t exp); */

seq_t a(idx_t n)
{
    /* Gamma: compute sigma(n) and phi(sigma(n)) */
    if (n == 0) return 0;                 /* below offset – arbitrary */
    seq_t sigma_n = sigma_of(n);
    seq_t phi_sigma = calc_phi(sigma_n);

    /* Factor n into prime powers */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* Compute product of phi(sigma(p^e)) over all prime powers */
    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < pcnt; ++i)
    {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i];
        /* sigma(p^e) = (p^(e+1) – 1)/(p – 1) */
        seq_t power  = pow_uint(p, e + 1);       /* p^(e+1)   */
        seq_t sigma_p = (power - (seq_t)1) / (p - 1);
        seq_t phi_sigma_p = calc_phi(sigma_p);
        prod *= phi_sigma_p;                    /* multiply the factor */
    }

    /* Final result: phi(sigma(n)) – product */
    return phi_sigma - prod;
}

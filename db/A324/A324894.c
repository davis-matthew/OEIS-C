/*
 * A324894  Multiplicative with a(p^e) = p^e if sigma(p^e) is composite, and 1
 *          otherwise.
 * https://oeis.org/A324894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   A324894 – multiplicative with a(p^e) = p^e if σ(p^e) is
 *            composite, 1 otherwise
 * Range:     u8 n <= 16, u16 n <= 4225, u32 n <= 9551, u64 n <= 9551,
 *            u128 n <= 9551, big any n
 * Helpers:   factor_u64, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n ≥ 1 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Factor n into primes and exponents */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;

    /* factor_u64 fills primes[], exps[] and sets cnt */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Multiplicative product of prime powers where σ(p^e) is composite */
    seq_t prod = 1;

    for (size_t i = 0; i < cnt; i++) {
        seq_t p = (seq_t)primes[i];
        int e = (int)exps[i];

        /* Compute p^e and (p^(e+1) - 1)/(p - 1) = σ(p^e) */
        seq_t p_pow_e = 1;
        for (int j = 0; j < e; j++) p_pow_e *= p;
        seq_t p_pow_e_plus1 = p_pow_e * p;
        seq_t sigma = (p_pow_e_plus1 - 1) / (p - 1);

        /* Include the factor if sigma is composite */
        if (!is_prime_big(sigma)) {
            prod *= p_pow_e;
        }
    }

    return prod;
}

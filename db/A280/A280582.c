/*
 * A280582  a(n) = the product of divisors of product of divisors of n.
 * https://oeis.org/A280582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of divisors of product of divisors of n
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* n must be >= 1 */
    if (n < 1)
        return 0;

    uint64_t n64 = (uint64_t)n;

    /* Factorise n: primes[i] ^ exps[i]  */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t primes_cnt = 0;
    factor_u64(n64, primes, exps, &primes_cnt);

    /* n = 1 has no prime factors: a(1) = 1 */
    if (primes_cnt == 0)
        return (seq_t)1;

    /* τ(n) = ∏ (e_i + 1) */
    seq_t tau_n = 1;
    for (size_t i = 0; i < primes_cnt; ++i)
        tau_n *= (seq_t)(exps[i] + 1);

    /* E_i = e_i * τ(n) / 2  (= exponent of p_i in D(n) = product of divisors of n) */
    seq_t *E = (seq_t *)malloc(primes_cnt * sizeof(seq_t));
    for (size_t i = 0; i < primes_cnt; ++i)
        E[i] = ((seq_t)exps[i] * tau_n) / 2;

    /* τ(m) where m = D(n) = ∏ p_i^{E_i} */
    seq_t tau_m = 1;
    for (size_t i = 0; i < primes_cnt; ++i)
        tau_m *= (E[i] + 1);

    /* Final result: a(n) = ∏ p_i ^ (E_i * τ(m) / 2) */
    seq_t result = 1;
    for (size_t i = 0; i < primes_cnt; ++i) {
        seq_t exp_final = (E[i] * tau_m) / 2;
        seq_t pow_val   = big_pow((seq_t)primes[i], exp_final);
        result *= pow_val;
    }

    free(E);
    return result;
}

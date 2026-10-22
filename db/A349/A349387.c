/*
 * A349387  Dirichlet convolution of A003961 with A055615 (Dirichlet inverse
 *          of n), where A003961 is fully multiplicative with a(p) =
 *          nextprime(p).
 * https://oeis.org/A349387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} A003961(n/d) * A055615(d) For multiplicative
 *            computation: a(p^e) = nextprime(p)^e - p * nextprime(p)^(e-1)
 *            and a(n) = ∏_{p^e‖n} a(p^e) (product over prime powers)
 * Range:     u8 n <= 127, u16 n <= 4095, u32 n <= 9929, u64 n <= 9929,
 *            u128 n <= 9929, big any n
 * Helpers:   big_pow, factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* compute nextprime(p) */
        uint64_t np = next_prime(p);  /* returns the smallest prime > p */

        seq_t np_pow_e      = big_pow((seq_t)np, (seq_t)e);
        seq_t np_pow_e_minus= big_pow((seq_t)np, (seq_t)(e - 1));
        seq_t term = np_pow_e - (seq_t)p * np_pow_e_minus;

        result *= term;
    }
    return result;
}

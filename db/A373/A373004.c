/*
 * A373004  a(n) = Sum_{k=1..n} sigma( (n/gcd(k,n))^n ).
 * https://oeis.org/A373004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(d) * sigma(d^n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, build_primes_upto, factor_u64, phi_from_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* external functions from utils.h */
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"
#include "../utils/phi_from_factors.h"

/* Static state for prime generation */
static uint64_t *s_primes = NULL;
static size_t   s_prime_cnt = 0;
static idx_t    s_prime_lim = 0;

/* helper: compute sigma(d^n) from its prime factorisation */
static seq_t sigma_power(idx_t d, idx_t n, uint64_t *fac, uint64_t *exp, size_t cnt)
{
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p   = (seq_t)fac[i];
        seq_t e   = (seq_t)exp[i] * (seq_t)n;          /* exponent in d^n           */
        seq_t ep1 = e + (seq_t)1;                      /* e+1                       */
        seq_t num = big_pow(p, ep1) - (seq_t)1;         /* p^(e+1) - 1               */
        seq_t den = p - (seq_t)1;                      /* p - 1                     */
        seq_t part = num / den;                        /* (p^(e+1)-1)/(p-1)         */
        result *= part;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* ensure we have enough primes for factorisation up to n */
    if (n > s_prime_lim) {
        build_primes_upto(n, &s_primes, &s_prime_cnt);
        s_prime_lim = n;
    }

    seq_t sum = (seq_t)0;
    /* loop over all divisors d of n */
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d != 0) continue;

        /* factorise d */
        uint64_t fac[16];
        uint64_t exp[16];
        size_t cnt;
        factor_u64((uint64_t)d, fac, exp, &cnt);

        /* phi(d) */
        seq_t phi = phi_from_factors(d, fac, exp, cnt);

        /* sigma(d^n) */
        seq_t sigma = sigma_power(d, n, fac, exp, cnt);

        sum += phi * sigma;
    }
    return sum;
}

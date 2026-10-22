/*
 * A397733  a(n) = Sum_{k=1..n} gcd(k^5,n).
 * https://oeis.org/A397733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{k=1..n} gcd(k^5 , n) The function uses the
 *            multiplicative property: a(p^e) = p^e + Σ_{k=0..e-1}
 *            (p^(e-k)-p^(e-k-1)) · p^min(5k,e) and a(n) = Π_{p^e ‖ n} a(p^e)
 * Range:     u8 n <= 26, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 1)
        return (seq_t)1;

    /* factor n into prime powers */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t prime_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &prime_cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p          = primes[i];
        idx_t    e          = (idx_t)exps[i];

        /* p^e */
        seq_t pe = pow_uint(p, e);

        /* sum over k = 0 .. e-1 */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < e; ++k) {
            /* p^(e-k) - p^(e-k-1) */
            seq_t pow_e_k     = pow_uint(p, e - k);
            seq_t pow_e_km1   = pow_uint(p, e - k - 1);
            seq_t diff        = pow_e_k - pow_e_km1;

            /* p^min(5k, e) */
            idx_t exp_min = (5 * k < e) ? (5 * k) : e;
            seq_t pk = pow_uint(p, exp_min);

            sum += diff * pk;
        }

        /* contribution of this prime power */
        seq_t term = pe + sum;
        result *= term;
    }

    return result;
}

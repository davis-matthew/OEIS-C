/*
 * A341529  a(n) = sigma(n) * A003961(n), where A003961 shifts the prime
 *          factorization of n one step towards larger primes, and sigma is
 *          the sum of the divisors of n.
 * https://oeis.org/A341529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = σ(n) * A003961(n) where σ(n) is the sum of divisors of n,
 *            and A003961(n) is the product of larger primes in the prime
 *            factorization of n: A003961(p^e) = next_prime(p)^e The sequence
 *            is multiplicative, so we compute it by factoring n
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 4095, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   factor_u64, next_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Handle n = 1 explicitly; no factors */
    if (n == 1) return (seq_t)1;

    /* Arrays for prime factors and exponents */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;

    /* Factor n */
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t sigma = (seq_t)1;          /* σ(n) */
    seq_t a003961 = (seq_t)1;        /* A003961(n) */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];

        /* σ part: (p^(e+1)-1)/(p-1) */
        seq_t term = (pow_uint(p, (idx_t)(e + 1)) - (seq_t)1) / (seq_t)(p - 1);
        sigma *= term;

        /* A003961 part: next_prime(p)^e */
        uint64_t q = next_prime(p);
        seq_t term2 = pow_uint(q, (idx_t)e);
        a003961 *= term2;
    }

    return sigma * a003961;
}

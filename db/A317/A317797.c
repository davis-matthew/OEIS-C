/*
 * A317797  Sum of the norm of divisors of n over Gaussian integers, with
 *          associated divisors counted only once.
 * https://oeis.org/A317797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, look
 * Formula:   a(n) = ∏_{p^e||n} f(p,e) where f(2,e) = 2^(2e+1) - 1 f(p,e) =
 *            ((p^(e+1)-1)/(p-1))^2 if p ≡ 1 (mod 4) f(p,e) =
 *            (p^(2e+2)-1)/(p^2-1) if p ≡ 3 (mod 4)
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 1712, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    /* Factor n into primes p_i and exponents e_i. */

    /* For n up to 64‑bit, at most 64 distinct primes – plenty. */
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   cnt   = 0;

    factor_u64(n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t part;

        if (p == 2ULL) {
            /* a(2^e) = 2^(2e+1) - 1 */
            seq_t pow = big_pow((seq_t)2ULL, (seq_t)(2ULL * e + 1ULL));
            part = pow - (seq_t)1;
        } else if ((p & 3U) == 1U) {
            /* f(p,e) = sigma(p^e)^2 where sigma(p^e) = (p^(e+1)-1)/(p-1) */
            seq_t sigma = (big_pow((seq_t)p, (seq_t)(e + 1ULL)) - (seq_t)1) / (seq_t)(p - 1);
            part = sigma * sigma;
        } else { /* p % 4 == 3 */
            /* f(p,e) = sigma_2(p^e) = (p^(2e+2)-1)/(p^2-1) */
            seq_t numerator = big_pow((seq_t)p, (seq_t)(2ULL * e + 2ULL)) - (seq_t)1;
            seq_t denom     = ((seq_t)p * (seq_t)p) - (seq_t)1;
            part = numerator / denom;
        }

        result *= part;
    }

    return result;
}

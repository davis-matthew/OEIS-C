/*
 * A353806  a(n) = A353802(n) / gcd(A051027(n), A353802(n)), where A051027(n)
 *          = sigma(sigma(n)), and A353802(n) = Product_{p^e||n}
 *          sigma(sigma(p^e)).
 * https://oeis.org/A353806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A353802(n) / gcd(A051027(n), A353802(n)) where A051027(n)
 *            = sigma(sigma(n)) A353802(n) = Π_{p^e||n} sigma(sigma(p^e)) with
 *            sigma_of(m) = sum of divisors of m and gcd(a,b) is the greatest
 *            common divisor
 * Range:     u8 n <= 29, u16 n <= 2529, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 111 terms through n = 9403 (2026-11-01)
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* compute A051027(n) = sigma(sigma(n)) */
    seq_t sigma_n = sigma_of((idx_t)n);
    seq_t sigma_sigma_n = sigma_of((idx_t)sigma_n);

    /* compute A353802(n) = product over prime factors p^e of sigma(sigma(p^e)) */
    seq_t prod = 1;

    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i)
    {
        /* compute p^e */
        seq_t pe = 1;
        for (uint64_t k = 0; k < exps[i]; ++k)
            pe *= primes[i];

        /* sigma(p^e) */
        seq_t sigma_pe = sigma_of((idx_t)pe);

        /* sigma(sigma(p^e)) */
        seq_t sigma_sigma_pe = sigma_of((idx_t)sigma_pe);

        prod *= sigma_sigma_pe;
    }

    /* divide by gcd with A051027(n) */
    seq_t g = gcd(prod, sigma_sigma_n);
    seq_t result = prod / g;
    return result;
}

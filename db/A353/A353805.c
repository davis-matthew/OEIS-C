/*
 * A353805  a(n) = A051027(n) / gcd(A051027(n), A353802(n)), where A051027(n)
 *          = sigma(sigma(n)), and A353802(n) = Product_{p^e||n}
 *          sigma(sigma(p^e)).
 * https://oeis.org/A353805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A051027(n) / gcd(A051027(n), A353802(n))
 * Range:     u8 n <= 29, u16 n <= 5014, u32 n <= 9982, u64 n <= 9982,
 *            u128 n <= 9982, big any n
 * Helpers:   factor_u64, gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/sigma_of.h"
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* A051027(n) = sigma(sigma(n)) */
    seq_t sigma_n = sigma_of((idx_t)n);
    seq_t A051   = sigma_of((idx_t)sigma_n);

    /* Compute product over prime powers */
    uint64_t N   = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;
    factor_u64(N, primes, exps, &cnt);

    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p_power = 1;
        for (uint64_t j = 0; j < exps[i]; ++j)
            p_power *= (seq_t)primes[i];

        seq_t sigma_p      = sigma_of((idx_t)p_power);
        seq_t sigma_sigma_p = sigma_of((idx_t)sigma_p);
        prod *= sigma_sigma_p;
    }

    /* Ratio */
    seq_t g = gcd(A051, prod);
    return A051 / g;
}

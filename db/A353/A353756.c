/*
 * A353756  a(n) = A353752(n) / gcd(A062401(n), A353752(n)), where A062401(n)
 *          = phi(sigma(n)), and A353752(n) = Product_{p^e||n}
 *          phi(sigma(p^e)).
 * https://oeis.org/A353756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A353752(n) / gcd(A062401(n), A353752(n)) where A353752(n)
 *            = ∏_{p^e || n} φ(σ(p^e)) A062401(n) = φ(σ(n))
 * Range:     u8 n <= 1144, u16 n <= 9980, u32 n <= 9980, u64 n <= 9980,
 *            u128 n <= 9980, big any n
 * Helpers:   factor_u64, gcd, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the helper routines from the utils
   library: factor_u64, sigma_of, phi and gcd.  All functions
   are cast to the build‑specific integer types (seq_t for the
   result and idx_t for indices).  The routine works for any
   n ≥ 1 and does not rely on any precomputed tables. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/sigma_of.h"
#include "../utils/phi.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case n == 1. */
    if (n == 1) return 1;

    /* Factorize n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute A353752(n) = product of φ(σ(p^e)) over prime powers. */
    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* Compute σ(p^e) = 1 + p + … + p^e. */
        seq_t sigma = 0;
        uint64_t pow = 1;
        for (uint64_t j = 0; j <= e; ++j) {
            sigma += pow;
            pow   *= p;
        }

        /* φ(σ(p^e)) */
        seq_t phi_sigma = phi((idx_t)sigma);
        prod *= phi_sigma;
    }

    /* Compute A062401(n) = φ(σ(n)). */
    seq_t sigma_n   = sigma_of(n);
    seq_t a062401   = phi((idx_t)sigma_n);

    /* Return A353752(n) / gcd(A062401(n), A353752(n)). */
    seq_t denom   = gcd(a062401, prod);
    return prod / denom;
}

/*
 * A347092  Dirichlet inverse of A322577, which is the convolution of Dedekind
 *          psi with Euler phi.
 * https://oeis.org/A347092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = 1 if n=1; otherwise a(n) = ∏_{p^e||n} f(p,e) where f(p,e)
 *            = 1 + p^2 if e is even, and f(p,e) = -2p if e is odd. This
 *            implements the multiplicative expression for OEIS A347092
 * Range:     i8 n <= 29, i16 n <= 2089, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];
        seq_t local;
        if ((e & 1) == 0) {
            local = p * p + (seq_t)1;
        } else {
            local = -(seq_t)2 * p;
        }
        result *= local;
    }
    return result;
}

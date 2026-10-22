/*
 * A327960  Dirichlet g.f.: 1 / (zeta(s) * zeta(s-1)^2).
 * https://oeis.org/A327960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a is multiplicative with a(p^e) = -(2p+1) if e = 1 p^2+2p if e =
 *            2 -p^2 if e = 3 0 otherwise. Thus a(1) = 1, and for general n we
 *            factor n and multiply the prime–power values. The values fit in
 *            the supplied seq_t type for all n in the intended range
 * Range:     i8 n <= 25, i16 n <= 1289, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;            /* a(1) = 1 */

    uint64_t N   = (uint64_t)n;            /* convert to native type for factoring */
    uint64_t pr[64], ex[64];
    size_t   cnt = 0;

    factor_u64(N, pr, ex, &cnt);          /* factor N into primes and exponents */

    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        seq_t val;

        if (e == 1) {
            val = -((seq_t)(2 * p + 1));
        } else if (e == 2) {
            val = (seq_t)p * p + 2 * (seq_t)p;
        } else if (e == 3) {
            val = -((seq_t)p * p);
        } else {
            /* exponent > 3 gives factor 0, so the whole product is 0 */
            return 0;
        }

        prod *= val;
    }

    return prod;
}

/*
 * A307424  Dirichlet g.f.: zeta(3*s) / zeta(2*s).
 * https://oeis.org/A307424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   multiplicative with a(p^e)=1 if e≡0 (mod 3), 0 if e≡1 (mod 3),
 *            -1 if e≡2 (mod 3)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
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
    if (n == 0) return 0;          /* undefined for n < 1, return 0 */

    uint64_t x = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factor_u64 writes prime factors of x into primes[] and their exponents
       into exps[], and sets cnt to the number of distinct primes. */
    factor_u64(x, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        uint64_t r = e % 3ULL;
        if (r == 1ULL) return 0;
        if (r == 2ULL) result = -result;
    }
    return result;
}

/*
 * A285335  Odd bisection of A048675 divided by two: a(n) =
 *          A048675((2*n)-1)/2.
 * https://oeis.org/A285335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A048675(2n-1)/2 = Sum_{p^e || 2n-1} e*2^(PrimePi(p)-2)
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 69, u64 n <= 158,
 *            u128 n <= 366, big any n
 * Validated: standard, pass, 79 terms through n = 993 (2026-11-08)
 * Helpers:   factor_u64, pow2, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/pow2.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    uint64_t t = 2ULL * (uint64_t)n - 1ULL;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    seq_t sum = 0;

    if (t <= 1ULL) return 0;

    factor_u64(t, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        /* t is odd, so every prime p >= 3 and PrimePi(p) >= 2 */
        idx_t pi = (idx_t)prime_pi((idx_t)primes[i]);
        seq_t term = pow2(pi - 2);
        term *= (seq_t)exps[i];
        sum += term;
    }
    return sum;
}

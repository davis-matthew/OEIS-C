/*
 * A365552  The number of exponentially odd divisors of the powerful part of
 *          n.
 * https://oeis.org/A365552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over prime powers p^e || n of f(e) where f(1) =
 *            1, and f(e) = floor((e+3)/2) for e >= 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the multiplicative rule for OEIS A365552. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* prototype for factor_u64 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;          /* a(1) = 1 */

    /* Work on a 64‑bit copy of n for factorization. */
    uint64_t nn = (uint64_t)n;

    /* Arrays to hold prime factors and exponents.
       At most log2(n) primes for n <= 10^4, so 64 is ample. */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;

    factor_u64(nn, pr, ex, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = ex[i];
        if (e == 1)
            continue;            /* multiplier is 1 */
        /* floor((e+3)/2) = (e+3)/2 integer division */
        result *= (e + 3) / 2;
    }
    return result;
}

/*
 * A391184  The number of divisors of n that are of the form x^2 + 2*y^2
 *          (A002479).
 * https://oeis.org/A391184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product_{p^e∥n} f(p,e) where f(p,e) = floor(e/2)+1 * if p
 *            ≡ 5 or 7 (mod 8), else f(p,e) = e+1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Array sizes large enough for any uint64_t factorization. */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt;

    /* factor_u64 works on uint64_t; n is idx_t (uint64_t). */
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        seq_t factor;

        uint64_t mod = p & 7U;               /* p % 8 */
        if (mod == 5U || mod == 7U) {
            factor = (seq_t)(e / 2U) + (seq_t)1U;
        } else {
            factor = (seq_t)e + (seq_t)1U;
        }

        result *= factor;
    }
    return result;
}

/*
 * A244339  Expansion of (-2 * a(q) + 3*a(q^2) + 2*a(q^4)) / 3 in powers of q
 *          where a() is a cubic AGM theta function.
 * https://oeis.org/A244339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -4 * b(n) where b is multiplicative: b(2^e) =
 *            (1-(-1)^e)*(-3)/4 for e>0, b(3^e) = 1, b(p^e) = e+1 if p ≡ 1
 *            (mod 6), b(p^e) = (1+(-1)^e)/2 if p ≡ 5 (mod 6)
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
    if (n == 0)
        return 1;                 /* a(0) = 1 */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* exponent of 2 */
    uint64_t e2 = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] == 2) {
            e2 = exps[i];
            break;
        }
    }

    /* when 2 appears with even exponent, a(n)=0 */
    if (e2 > 0 && (e2 & 1) == 0)
        return 0;

    /* product of the integer parts of b(p^e) for primes ≠ 2 */
    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        if (p == 2)
            continue;
        if (p == 3)
            continue;               /* b(3^e) = 1, no effect */

        if (p % 6 == 1) {
            prod *= (seq_t)(e + 1);
        } else if (p % 6 == 5) {
            if (e & 1)
                return 0;           /* b gives 0 for odd exponent */
            /* even exponent gives factor 1, nothing to multiply */
        }
    }

    if (e2 == 0)                    /* no factor 2 */
        return -4 * prod;
    else                            /* e2 odd */
        return 6 * prod;
}

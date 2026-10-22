/*
 * A342025  a(n) = 1 if n has the same numbers of prime factors of forms 4*k+1
 *          and 4*k+3 when counted with multiplicity, otherwise 0.
 * https://oeis.org/A342025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n has the same numbers of prime factors of forms
 *            4*k+1 and 4*k+3 (counted with multiplicity), otherwise 0
 * Range:     u8 n <= 9907, u16 n <= 9907, u32 n <= 9907, u64 n <= 9907,
 *            u128 n <= 9907, big any n
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
        return 0; /* undefined case, return 0 */
    if (n == 1)
        return 1; /* zero prime factors → equal counts */

    uint64_t tn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(tn, primes, exps, &cnt);

    unsigned cnt1 = 0, cnt3 = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        if (p == 2)
            continue; /* 2 ≡ 2 (mod 4) → ignore */
        unsigned mod = p & 3;
        if (mod == 1)
            cnt1 += (unsigned)e;
        else if (mod == 3)
            cnt3 += (unsigned)e;
    }
    return (seq_t)(cnt1 == cnt3);
}

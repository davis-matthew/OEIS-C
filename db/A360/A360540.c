/*
 * A360540  a(n) is the cubefull part of n: the largest divisor of n that is a
 *          cubefull number (A036966).
 * https://oeis.org/A360540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏ p^e | n of p^e if e ≥ 3 (else 1)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if (ex[i] >= 3) {
            seq_t base = (seq_t)pr[i];
            for (uint64_t e = 0; e < ex[i]; ++e)
                result *= base;
        }
    }
    return result;
}

/*
 * A375033  The maximum even exponent in the prime factorization of n, or 0 if
 *          no such exponent exists.
 * https://oeis.org/A375033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = maximum even exponent in the prime factorization of n, 0
 *            if none
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    uint64_t N = (uint64_t)n;
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt;
    factor_u64(N, pr, ex, &cnt);
    seq_t maxEven = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = ex[i];
        if ((e & 1) == 0) {            // even exponent
            seq_t ev = (seq_t)e;
            if (ev > maxEven) maxEven = ev;
        }
    }
    return maxEven;
}

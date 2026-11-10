/*
 * A373141  a(n) = 1 if A276085(n) is of the form 4m+2, otherwise 0, where
 *          A276085 is the primorial base log-function.
 * https://oeis.org/A373141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A276085(n) == 2 (mod 4), else 0. With e = v2(n) and
 *            m = bigomega(odd part of n): A276085(n) == e + 2*m (mod 4), so
 *            a(n) = 1 iff e is even and e/2 + m is odd
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 141 terms through n = 9871 (2026-11-10)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    uint64_t u = (uint64_t)n;
    uint64_t e = 0;
    if (u == 0) return (seq_t)0;
    while ((u & 1) == 0) { u >>= 1; ++e; }
    uint64_t m = 0;
    if (u > 1) {
        uint64_t facs[64];
        uint64_t exps[64];
        size_t cnt = 0;
        factor_u64(u, facs, exps, &cnt);
        for (size_t i = 0; i < cnt; ++i)
            m += exps[i];
    }
    if (e & 1) return (seq_t)0;
    return (seq_t)(((e / 2 + m) & 1) ? 1 : 0);
}

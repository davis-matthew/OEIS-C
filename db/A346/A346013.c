/*
 * A346013  a(n) is the denominator of A346009(n)/A346010(n) - A001221(n)/2.
 * https://oeis.org/A346013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of Sum_{p^e||n} (e-1)/(2*(e+1)), in lowest
 *            terms; a(1) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 101 terms through n = 8800 (2026-11-10)
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t L = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if (ex[i] <= 1) continue;
        seq_t d = (seq_t)(2 * (ex[i] + 1));
        seq_t g = gcd(L, d);
        L = (L / g) * d;
    }

    seq_t num = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (ex[i] <= 1) continue;
        seq_t d = (seq_t)(2 * (ex[i] + 1));
        num += (seq_t)(ex[i] - 1) * (L / d);
    }

    if (num == 0) return 1;
    seq_t g = gcd(num, L);
    return L / g;
}

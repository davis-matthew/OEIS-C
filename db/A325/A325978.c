/*
 * A325978  a(n) = (1/2)*(A325314(n) + A325814(n)).
 * https://oeis.org/A325978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n - (1/2)*(A048146(n) + A162296(n)), computed
 *            multiplicatively: with sigma(n), usigma(n)=prod(1+p^e),
 *            r(n)=prod(1+p): A048146 = sigma - usigma, A162296 = sigma - r,
 *            so a(n) = n - sigma + (usigma + r)/2
 * Range:     i8 n <= 83, i16 n <= 17279, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 101 terms through n = 9963 (2026-11-08)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t sig = 1, usig = 1, rad = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)pr[i];
        seq_t pw = 1, s = 1;
        for (uint64_t e = 0; e < ex[i]; ++e) {
            pw *= p;
            s += pw;
        }
        sig *= s;
        usig *= (1 + pw);
        rad *= (1 + p);
    }
    return (seq_t)n - sig + (usig + rad) / 2;
}

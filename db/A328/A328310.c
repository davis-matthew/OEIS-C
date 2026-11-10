/*
 * A328310  a(n) = A051903(A003415(n)) - A051903(n).
 * https://oeis.org/A328310
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A051903(A003415(n)) - A051903(n), where A051903 = maximum
 *            exponent in prime factorization (0 for 1), A003415 = arithmetic
 *            derivative
 * Range:     i8 n <= 63, i16 n <= 20000, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 110 terms through n = 9101 (2026-11-08)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static seq_t A328310_maxexp_seq(seq_t m)
{
    /* maximum exponent in factorization of m, for m too large for factor_u64 */
    if (m <= 1) return (seq_t)0;
    seq_t best = 0;
    seq_t rem = m;
    for (seq_t p = 2; p * p <= rem; p++) {
        seq_t e = 0;
        while (rem % p == 0) {
            rem /= p;
            e++;
        }
        if (e > best) best = e;
    }
    if (rem > 1 && best < 1) best = 1;
    return best;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t maxn = 0;
    seq_t d = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t e = (seq_t)ex[i];
        if (e > maxn) maxn = e;
        d += e * (seq_t)(n / pr[i]);
    }

    seq_t maxd = 0;
    if (d <= (idx_t)(((idx_t)1) << 62)) {
        uint64_t du = (uint64_t)(long long)d;
        uint64_t pr2[64], ex2[64];
        size_t c2 = 0;
        if (du > 1) {
            factor_u64(du, pr2, ex2, &c2);
            for (size_t i = 0; i < c2; ++i) {
                seq_t e = (seq_t)ex2[i];
                if (e > maxd) maxd = e;
            }
        }
    } else {
        maxd = A328310_maxexp_seq(d);
    }

    return maxd - maxn;
}

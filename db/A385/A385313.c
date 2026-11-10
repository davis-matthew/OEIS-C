/*
 * A385313  a(n) = c(n) + Sum_{d|n} d * phi(n/d) * (1 - c(d)), where c =
 *          A010051.
 * https://oeis.org/A385313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = c(n) + Sum_{d|n} d*phi(n/d)*(1-c(d)), c = A010051;
 *            computed as c(n) + P(n) - Sum_{p|n, p prime} p*phi(n/p), where P
 *            is the multiplicative Pillai function, P(p^k) = (k+1)p^k - k
 *            p^(k-1)
 * Range:     u8 n <= 59, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Validated: partial, pass, 84 terms through n = 84 (2026-11-10)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t P = 1;
    seq_t phi = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)pr[i];
        seq_t pe1 = 1; /* p^(e-1) */
        for (uint64_t j = 1; j < ex[i]; ++j) pe1 *= p;
        seq_t e = (seq_t)ex[i];
        P *= (e + 1) * pe1 * p - e * pe1;
        phi *= pe1 * (p - 1);
    }

    seq_t S = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)pr[i];
        if (ex[i] >= 2) S += phi;
        else S += p * (phi / (p - 1));
    }

    seq_t c = (cnt == 1 && ex[0] == 1) ? 1 : 0;
    return c + P - S;
}

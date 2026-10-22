/*
 * A341832  Dirichlet g.f.: 1 / zeta(s)^6.
 * https://oeis.org/A341832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = \prod_{p^e \parallel n} (-1)^e * binomial(6, e)
 *            (multiplicative)
 * Range:     i8 n <= 29, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   binomial, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;

    uint64_t val = (uint64_t)n;
    uint64_t pf[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(val, pf, ex, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t e = (idx_t)ex[i];
        seq_t term = binomial(6, e);   /* binomial(6, e) is zero for e > 6 */
        if (e & 1) term = -term;
        result *= term;
    }
    return result;
}

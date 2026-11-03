/*
 * A387432  a(n) = Sum_{k=0..n} binomial(n,k) * binomial(4*n+3*k,k).
 * https://oeis.org/A387432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * binomial(4*n+3*k,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 594 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = binomial(n, k);
        term *= binomial(4 * n + 3 * k, k);
        res += term;
    }
    return res;
}

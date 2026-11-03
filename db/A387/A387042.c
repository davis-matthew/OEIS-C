/*
 * A387042  a(n) = Sum_{k=0..n} binomial((n+k)*n, (n+k)*k).
 * https://oeis.org/A387042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial((n+k)*n, (n+k)*k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 11 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation relies on the provided utility function `binom(idx_t, idx_t)`. */
#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t N = (n + k) * n;   /* (n+k)*n */
        idx_t K = (n + k) * k;   /* (n+k)*k */
        sum += binomial(N, K);
    }
    return sum;
}

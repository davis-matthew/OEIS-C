/*
 * A372376  Expansion of (1/x) * Series_Reversion( x * (1+x) / (1+x+x^3)^3 ).
 * https://oeis.org/A372376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..⌊n/3⌋} binom(3*n+3,k) *
 *            binom(2*n−k+2,n−3*k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t c1 = binomial(3*n + 3, k);
        seq_t c2 = binomial(2*n - k + 2, n - 3*k);
        sum += c1 * c2;
    }
    seq_t denom = (seq_t)(n + 1);
    return sum / denom;
}

/*
 * A374598  Expansion of 1/sqrt(1 - 4*x - 8*x^3).
 * https://oeis.org/A374598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} 2^k * binom(n-2*k, k) *
 *            binom(2*(n-2*k), n-2*k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 987 (2026-11-01)
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t n2k = n - 2 * k;                 /* n-2k */
        seq_t term = pow2(k);
        term *= binomial(n2k, k);
        term *= binomial(2 * n2k, n2k);
        sum += term;
    }
    return sum;
}

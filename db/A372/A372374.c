/*
 * A372374  Coefficient of x^n in the expansion of (1+x+x^3)^(3*n).
 * https://oeis.org/A372374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(3*n,k) *
 *            binomial(3*n-k,n-3*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t max_k = n / 3;
    idx_t total_3n = 3 * n;
    for (idx_t k = 0; k <= max_k; ++k) {
        seq_t term1 = binomial(total_3n, k);
        seq_t term2 = binomial(total_3n - k, n - 3 * k);
        sum += term1 * term2;
    }
    return sum;
}

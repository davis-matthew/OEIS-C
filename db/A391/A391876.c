/*
 * A391876  a(n) = Sum_{k=0..n} 2^(n-k) * binomial(k+2,2) *
 *          binomial(2*k,2*(n-k)).
 * https://oeis.org/A391876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} 2^(n-k) * binomial(k+2, 2) * binomial(2*k,
 *            2*(n-k))
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 34, u128 n <= 70,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 978 (2026-11-01)
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
    for (idx_t k = 0; k <= n; ++k) {
        seq_t pw   = pow2(n - k);               /* 2^(n-k) */
        seq_t term = pw * binomial(k + 2, 2);      /* times binomial(k+2, 2) */
        term = term * binomial(2 * k, 2 * (n - k));
        sum = sum + term;
    }
    return sum;
}

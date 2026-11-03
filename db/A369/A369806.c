/*
 * A369806  Expansion of 1/(1 - x^3/(1-x)^7).
 * https://oeis.org/A369806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n-1+4*k, n-3*k)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 30 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 3; ++k) {
        seq_t term = binomial(n - 1 + 4 * k, n - 3 * k);
        sum += term;
    }
    return sum;
}

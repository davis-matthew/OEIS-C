/*
 * A382257  a(n) is the numerator of tanh(Sum_{k=1..n-1} artanh(k/n)), where
 *          artanh is the inverse hyperbolic tangent function.
 * https://oeis.org/A382257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = (binomial(2n-1, n-1) - 1)/2 if n is a power of 2,
 *            otherwise a(n) = binomial(2n-1, n-1) - 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-01)
 * Helpers:   binomial, is_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/is_pow2.h"

seq_t a(idx_t n)
{
    seq_t bin = binomial(2 * n - 1, n - 1);
    if (is_pow2((seq_t)n))
        return (bin - 1) / 2;
    else
        return bin - 1;
}

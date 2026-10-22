/*
 * A382257  a(n) is the numerator of tanh(Sum_{k=1..n-1} artanh(k/n)), where
 *          artanh is the inverse hyperbolic tangent function.
 * https://oeis.org/A382257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = (binomial(2n-1, n-1) - 1)/2 if n is a power of 2,
 *            otherwise a(n) = binomial(2n-1, n-1) - 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   binom, is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/is_power_of_two.h"

seq_t a(idx_t n)
{
    seq_t bin = binom(2 * n - 1, n - 1);
    if (is_power_of_two((seq_t)n))
        return (bin - 1) / 2;
    else
        return bin - 1;
}

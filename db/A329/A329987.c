/*
 * A329987  Beatty sequence for the number x satisfying 1/x + 1/2^x = 1.
 * https://oeis.org/A329987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * x), where x satisfies 1/x + 1/2^x = 1 (x ≈
 *            1.52980838275...)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   _compute_x
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>
#include "../utils/_compute_x.h"

/* Compute the constant x once, using a binary search in long double precision. */
static bool _x_computed = false;
static long double _x_value;

seq_t a(idx_t n)
{
    if (!_x_computed) {
        _x_value = _compute_x();
        _x_computed = true;
    }

    /* Compute floor(n * x) with a tiny epsilon to avoid rounding up due to FP error. */
    long double prod = (long double)n * _x_value;
    unsigned long long val = (unsigned long long)floorl(prod + 1e-12L);
    return (seq_t)val;
}

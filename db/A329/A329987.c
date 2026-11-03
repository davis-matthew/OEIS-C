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
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>

static long double _compute_x(void)
{
    long double lo = 1.0L, hi = 2.0L;          /* root lies between 1 and 2 */
    for (int i = 0; i < 80; ++i)                /* 80 rounds gives > 1e-19 accuracy */
    {
        long double mid = (lo + hi) * 0.5L;
        long double f = 1.0L / mid + powl(2.0L, -mid) - 1.0L; /* f(mid) = 0 at root */
        if (f > 0.0L)
            lo = mid;
        else
            hi = mid;
    }
    return hi;
}

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

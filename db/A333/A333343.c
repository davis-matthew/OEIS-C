/*
 * A333343  Least k such that f(k) < 0, where f(1) = n and f(k) =
 *          f(k-1)*2^(1/k) - 1 for k > 1.
 * https://oeis.org/A333343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that f(k) < 0, where f(1) = n and f(k) =
 *            f(k-1) * 2^(1/k) - 1 for k > 1
 * Range:     u8 n <= 18, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    long double f = (long double)n;
    for (idx_t k = 2; ; ++k)
    {
        f = f * powl(2.0L, 1.0L / (long double)k) - 1.0L;
        if (f < 0.0L)
            return (seq_t)k;
    }
}

/*
 * A309126  a(n) = n + 2^3 * floor(n/2^3) + 3^3 * floor(n/3^3) + 4^3 *
 *          floor(n/4^3) + ...
 * https://oeis.org/A309126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..⌊n^{1/3}⌋} k^3 · ⌊n / k^3⌋ (OEIS offset 1)
 * Range:     u8 n <= 71, u16 n <= 4479, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9493 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

/* integer cube root: largest k with k³ ≤ x */
static idx_t int_cube_root(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        __int128 cube = (__int128)mid * mid * mid;
        if (cube <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum = 0;
    idx_t maxk = int_cube_root(n);   /* floor(cuberoot(n)) */

    for (idx_t k = 1; k <= maxk; ++k)
    {
        idx_t k3 = k * k * k;        /* k³ ≤ n, fits in idx_t */
        idx_t q  = n / k3;           /* floor(n / k³) */
        sum += (seq_t)k3 * (seq_t)q;
    }

    return sum;
}

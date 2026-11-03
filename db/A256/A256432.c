/*
 * A256432  Characteristic function of octahedral numbers.
 * https://oeis.org/A256432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is an octahedral number (2*m^3+m)/3 for some
 *            integer m, else 0
 * Range:     u8 n <= 42, u16 n <= 9879, u32 n <= 104994, u64 n <= 104994,
 *            u128 n <= 104994, big any n
 * Validated: standard, pass, 102 terms through n = 9879 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static seq_t _octahedral_value(seq_t m)
{
    /* Compute (2*m^3 + m)/3 using only integer arithmetic. */
    seq_t m2 = m * m;
    seq_t m3 = m2 * m;
    seq_t numer = (seq_t)2 * m3 + m;
    return numer / (seq_t)3;
}

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;

    /* First approximation of m = floor((3*N)/2)^(1/3). */
    seq_t approx = ((seq_t)3 * N) / 2;
    seq_t m     = floor_cube_root(approx);

    /* Check whether N is exactly (2*m^3 + m)/3. */
    if (_octahedral_value(m) == N)
        return 1;
    else
        return 0;
}

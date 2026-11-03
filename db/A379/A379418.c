/*
 * A379418  a(n) = n + [n*r/s] + [n*t/s], where r = (3/2)^(1/4); s =
 *          (3/2)^(1/2); t = (3/2)^(3/4) and [ ] = floor.
 * https://oeis.org/A379418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n/r) + floor(n*r), r = (3/2)^(1/4)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer 4th‑root by binary search (root^4 ≤ x < (root+1)^4) */
static idx_t floor_root4_idx(idx_t x)
{
    const idx_t MAX_ROOT = 65535;          /* floor_root4(2^64-1) */
    idx_t low  = 0;
    idx_t high = MAX_ROOT;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        idx_t mid2 = mid * mid;
        idx_t mid4 = mid2 * mid2;
        if (mid4 <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* floor(n/r) = floor( [(2*n^4)/3]^(1/4) ) */
static idx_t floor_n_over_r(idx_t n)
{
    idx_t n2 = n * n;
    idx_t n4 = n2 * n2;
    idx_t val = (2 * n4) / 3;
    return floor_root4_idx(val);
}

/* floor(n*r) = floor( [(3*n^4)/2]^(1/4) ) */
static idx_t floor_n_r(idx_t n)
{
    idx_t n2 = n * n;
    idx_t n4 = n2 * n2;
    idx_t val = (3 * n4) / 2;
    return floor_root4_idx(val);
}

/* OEIS sequence A379418 */
seq_t a(idx_t n)
{
    if (n == 0) /* safety, offset is 1 */
        return 0;

    seq_t term      = (seq_t)n;
    seq_t term_low  = (seq_t)floor_n_over_r(n);
    seq_t term_high = (seq_t)floor_n_r(n);

    return term + term_low + term_high;
}

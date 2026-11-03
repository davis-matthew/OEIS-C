/*
 * A372511  Number of solutions to x^2 + y^2 <= n, where x, y are positive odd
 *          integers.
 * https://oeis.org/A372511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 85 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of pairs of positive odd integers (x,y) such that x^2 + y^2 <= n */
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* largest possible odd x */
    idx_t sqrt_n = (idx_t)floor_sqrt((seq_t)n);
    seq_t result = 0;

    for (idx_t x = 1; x <= sqrt_n; x += 2) {
        idx_t sq = x * x;            /* x^2 ≤ n */
        idx_t rem = n - sq;          /* remaining room for y^2 */

        if (rem < 1)                /* y must be positive odd integer */
            continue;

        idx_t m = (idx_t)floor_sqrt((seq_t)rem);
        seq_t cnt = (seq_t)(m + 1) / 2;   /* count of odd y ≤ m */
        result += cnt;
    }

    return result;
}

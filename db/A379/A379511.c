/*
 * A379511  a(n) = floor(n*(2^(1/4) + 2^(-1/4))); Beatty sequence for 2^(1/4)
 *          + 2^(-1/4).
 * https://oeis.org/A379511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(2^(1/4)+2^(-1/4)))
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Validated: standard, pass, 78 terms through n = 9984 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

static const seq_t SCALE = (seq_t)1000000000ULL;
static seq_t pre_sqrt2_scaled = 0;
static seq_t pre_E_scaled = 0;

static void init_constants(void)
{
    if (!pre_sqrt2_scaled)
    {
        /* Compute floor(sqrt(2) * SCALE) */
        seq_t temp = 2 * SCALE * SCALE;
        pre_sqrt2_scaled = floor_sqrt(temp);          /* t1 = floor(sqrt(2) * SCALE) */

        /* Compute E_scaled = floor( (2 + (3/2)*sqrt(2)) * SCALE ) */
        seq_t term = (3 * pre_sqrt2_scaled) / 2;      /* floor( (3/2)*sqrt(2)*SCALE ) */
        pre_E_scaled = 2 * SCALE + term;              /* floor( (2 + (3/2)*sqrt(2))*SCALE ) */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n < 1, safe guard */
    init_constants();

    seq_t nn = (seq_t)n;
    seq_t nn_sq = nn * nn;          /* n^2 */
    seq_t M = nn_sq * pre_E_scaled; /* n^2 * E_scaled */
    seq_t Yfloor = M / SCALE;       /* floor(n^2 * E) */
    return floor_sqrt(Yfloor);      /* floor(sqrt(n^2 * E)) = floor(n*u) */
}

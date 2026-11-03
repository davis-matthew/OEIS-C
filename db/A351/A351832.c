/*
 * A351832  Least nonnegative integer m such that n = x^6 + y^6 - (z^2 + m^2)
 *          for some nonnegative integers x,y,z with z <= m.
 * https://oeis.org/A351832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least nonnegative integer m such that n = x^6 + y^6 -
 *            (z^2 + m^2) for some nonnegative integers x, y, z with z ≤ m.
 *            The code searches increasing m and tests all admissible x, y, z
 *            by a direct brute‑force that is easily fast enough for n up to
 *            at least 10000
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 110 terms through n = 9601 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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

#define X_MAX 70          /* enough for m up to ~10^5   */
static bool x6_initialized = false;
static seq_t x6[X_MAX + 1];

static void init_x6(void) {
    if (x6_initialized) return;
    for (int i = 0; i <= X_MAX; ++i) {
        seq_t val = 1;
        for (int j = 0; j < 6; ++j) val *= (seq_t)i;
        x6[i] = val;
    }
    x6_initialized = true;
}

seq_t a(idx_t n) {
    init_x6();
    const seq_t n_val = (seq_t)n;

    for (idx_t m = 0; ; ++m) {
        const seq_t M    = (seq_t)m * (seq_t)m;
        const seq_t bound = n_val + (seq_t)2 * M;   /* x^6 + y^6 ≤ n + 2m² */

        for (int x = 0; x <= X_MAX; ++x) {
            const seq_t xsq = x6[x];
            if (xsq > bound) break;

            const seq_t rem = bound - xsq;  /* maximum y^6 allowed */
            for (int y = 0; y <= X_MAX; ++y) {
                const seq_t ysq = x6[y];
                if (ysq > rem) break;

                const seq_t val = xsq + ysq - n_val - M;  /* z² */
                if (val > M) continue;               /* z² > m² impossible */
                if (val < 0) continue;               /* should not happen */

                const seq_t z = floor_sqrt(val);
                if (z * z == val && z <= (seq_t)m) {
                    return (seq_t)m;
                }
            }
        }
    }
}

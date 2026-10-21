/*
 * A273294  Least nonnegative integer m such that there are nonnegative
 *          integers x,y,z,w for which x^2 + y^2 + z^2 + w^2 = n and x + 3*y +
 *          5*z = m^2.
 * https://oeis.org/A273294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the smallest non‑negative integer m such that
   there exist non‑negative integers x,y,z,w with
   x² + y² + z² + w² = n  and  x + 3·y + 5·z = m².  */
/* The function does not use floating point arithmetic and works for any
   seq_t and idx_t types defined by the build. */
#include <stdint.h>
#include <stdbool.h>

/* Integer square‑root for an idx_t value.
   Uses only integer operations and works for all 64‑bit values. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t r = 0;
    while (r + 1 <= n / (r + 1))
        ++r;
    return r;
}

/* Computes a(n) as defined in OEIS A273294. */
seq_t a(idx_t n)
{
    /* No special handling for n == 0; the loop below covers it. */
    idx_t sqrt_n = isqrt_u64(n);
    idx_t best   = (idx_t)-1;      /* sentinel for "not found yet". */

    for (idx_t x = 0; x <= sqrt_n; ++x) {
        idx_t x2 = x * x;
        if (x2 > n) break;
        idx_t rest1 = n - x2;
        idx_t sqrt_rest1 = isqrt_u64(rest1);

        for (idx_t y = 0; y <= sqrt_rest1; ++y) {
            idx_t y2 = y * y;
            idx_t sum_xy = x2 + y2;
            if (sum_xy > n) break;
            idx_t rest2 = n - sum_xy;
            idx_t sqrt_rest2 = isqrt_u64(rest2);

            for (idx_t z = 0; z <= sqrt_rest2; ++z) {
                idx_t z2 = z * z;
                idx_t sum_xyz = sum_xy + z2;
                if (sum_xyz > n) break;
                idx_t w2 = n - sum_xyz;

                idx_t w = isqrt_u64(w2);
                if (w * w != w2) continue;   /* w² must equal w2 */

                idx_t t = x + 3 * y + 5 * z;
                idx_t m = isqrt_u64(t);
                if (m * m == t) {
                    if (best == (idx_t)-1 || m < best)
                        best = m;
                }
            }
        }
    }

    /* If no representation found (should not happen for n ≥ 0) return 0. */
    return (seq_t)(best == (idx_t)-1 ? 0 : best);
}

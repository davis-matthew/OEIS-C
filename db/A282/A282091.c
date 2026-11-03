/*
 * A282091  Number of ways to write n as x^2 + y^2 + z^2 + w^2 with x + y - z
 *          a cube of an integer, where x,y,z,w are nonnegative integers with
 *          x >= y <= z and x == y (mod 2).
 * https://oeis.org/A282091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9813 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = number of quadruples (x,y,z,w) of non‑negative integers
 * satisfying  x^2 + y^2 + z^2 + w^2 = n,
 *  x >= y <= z,
 *  x ≡ y (mod 2),
 *  and x + y - z is a perfect cube (of an integer). */

#include <stdbool.h>
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
    seq_t N = (seq_t)n;
    /* Largest possible value of any of x, y, z, w */
    idx_t sqrt_n = (idx_t)floor_sqrt(N);

    seq_t count = 0;

    /* Determine the range of cubes to try.
     * We need |k^3| <= sqrt_n + sqrt_n, because
     * x = k^3 + (z - y) and z - y <= sqrt_n. */
    idx_t cube_limit = sqrt_n + sqrt_n;
    int maxk = 0;
    while ((idx_t)((long long)(maxk + 1) * (maxk + 1) * (maxk + 1)) <= cube_limit)
        ++maxk;

    for (idx_t y = 0; y <= sqrt_n; ++y) {
        seq_t y2 = (seq_t)y * (seq_t)y;
        if (y2 > N) continue;         /* no point if y alone exceeds n */

        for (idx_t z = y; z <= sqrt_n; ++z) {
            seq_t z2 = (seq_t)z * (seq_t)z;
            seq_t sum_yz = y2 + z2;
            if (sum_yz > N) continue; /* same reasoning as above */

            /* d = z - y  (always non‑negative) */
            idx_t d = z - y;

            for (int k = -maxk; k <= maxk; ++k) {
                long long cubeLL = (long long)k * k * k;          /* k³ */
                long long xLL = cubeLL + d;                      /* x = k³ + (z - y) */

                if (xLL < 0 || (idx_t)xLL < y) continue;        /* x must be ≥ y and ≥ 0 */
                if ((idx_t)xLL > sqrt_n) continue;              /* x cannot exceed sqrt(n) */

                idx_t x = (idx_t)xLL;

                /* parity constraint: x ≡ y (mod 2) */
                if ( (x & 1) != (y & 1) ) continue;

                seq_t x2 = (seq_t)x * (seq_t)x;
                seq_t total = x2 + sum_yz;
                if (total > N) continue;                        /* ensures w² ≥ 0 */

                seq_t w2 = N - total;
                seq_t w = floor_sqrt(w2);
                if (w * w == w2)
                    ++count;
            }
        }
    }

    return count;
}

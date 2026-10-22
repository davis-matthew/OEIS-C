/*
 * A351341  Least nonnegative integer m such that n = x^4 + y^4 - (z^3 + m^3)
 *          for some nonnegative integers x,y,z with z <= m.
 * https://oeis.org/A351341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A351341: a(n) = least nonnegative integer m such that
 *    n = x^4 + y^4 - (z^3 + m^3)
 * for some nonnegative integers x, y, z with z ≤ m.
 *
 * The implementation searches m increasingly.  For each m it
 * determines an upper bound on x and y (fourth root of n + 2·m³)
 * and tries all pairs (x, y) with 0 ≤ y ≤ x ≤ limit.  For each pair
 * it checks whether the remainder D = x⁴ + y⁴ – n can be written
 * as z³ + m³ with z ≤ m.  A simple integer cube‑root via binary
 * search is used, so no floating‑point arithmetic is required.
 *
 * The code works for any of the supported seq_t types
 * (uint8/16/32/64 or arbitrary precision) and compiles in C11
 * and C++17.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Integer floor cube root for any seq_t using only integer ops. */
static inline seq_t cube_root_floor(seq_t v)
{
    seq_t low  = 0;
    seq_t high = 1;
    /* Double high until high³ > v */
    while (high * high * high <= v) {
        high <<= 1;
    }
    /* Binary search for floor cube root */
    while (low < high) {
        seq_t mid = (low + high + 1) >> 1;      /* upper middle */
        seq_t cube = mid * mid * mid;
        if (cube <= v) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

/* Integer floor fourth root: sqrt(sqrt(x)) */
static inline seq_t floor_pow4(seq_t x)
{
    return floor_sqrt(floor_sqrt(x));
}

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;

    for (idx_t m = 0;; ++m) {
        seq_t m_seq  = (seq_t)m;
        seq_t m3     = m_seq * m_seq * m_seq;      /* m³ */
        seq_t maxSum = n_seq + 2 * m3;             /* bound on x⁴ + y⁴ */
        idx_t lim = (idx_t)floor_pow4(maxSum);     /* limit for x, y */

        for (idx_t x = 0; x <= lim; ++x) {
            seq_t x2  = (seq_t)x * (seq_t)x;
            seq_t x4  = x2 * x2;                   /* x⁴ */

            for (idx_t y = 0; y <= x; ++y) {      /* use symmetry y ≤ x */
                seq_t y2  = (seq_t)y * (seq_t)y;
                seq_t y4  = y2 * y2;               /* y⁴ */
                seq_t sum = x4 + y4;

                if (sum > maxSum) break;          /* further y only increases sum */
                if (sum < n_seq) continue;

                seq_t D = sum - n_seq;             /* D = x⁴ + y⁴ – n */

                if (D < m3) continue;             /* need D ≥ m³ */
                seq_t rem = D - m3;                /* rem = z³  */
                if (rem > m3) continue;            /* z must ≤ m */

                seq_t z = cube_root_floor(rem);
                if (z > m) continue;              /* enforce z ≤ m */

                if (z * z * z == rem) {           /* verify perfect cube */
                    return m;                      /* minimal m found */
                }
            }
        }
        /* no representation for this m, try next */
    }
}

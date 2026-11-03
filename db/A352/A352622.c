/*
 * A352622  Number of regular convex polytopes that can be formed with n
 *          indistinguishable points located at the vertices, coinciding in
 *          equal frequency at each vertex, if coinciding at all.
 * https://oeis.org/A352622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A111336(d)
 * Range:     u8 n <= 1801, u16 n <= 1801, u32 n <= 1801, u64 n <= 1801,
 *            u128 n <= 1801, big any n
 * Validated: standard, pass, 94 terms through n = 1750 (2026-11-01)
 * Helpers:   is_pow2_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A111336(d) counts distinct regular convex polytopes with d vertices. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_pow2_idx.h"

/* Number of regular convex polytopes with exactly 'v' vertices. */
static inline seq_t _a111336(idx_t v)
{
    seq_t cnt = 1;               /* simplex (always present) */

    /* polygon exists for all v >= 3, except the triangle (v==3) which
       coincides with the simplex. */
    if (v >= 4u)
        cnt += 1;

    /* cross‑polytope (2d vertices) exists for any even v >= 6.  The case
       v==4 is already counted by the polygon. */
    if ((v & 1u) == 0u && v >= 6u)
        cnt += 1;

    /* hypercube (2^k vertices) exists for any v that is a power of two
       with v >= 8.  The cases v==2 and v==4 are already counted. */
    if (is_pow2_idx(v) && v >= 8u)
        cnt += 1;

    /* Exceptional 3‑D regular polytopes (not counted above). */
    if (v == 12u || v == 20u)
        cnt += 1;

    /* Exceptional 4‑D regular polytopes. */
    if (v == 24u || v == 120u || v == 600u)
        cnt += 1;

    return cnt;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0u)
        return 0;   /* undefined offset, but safe to return 0 */

    seq_t sum = 0;
    for (idx_t i = 1u; i * i <= n; ++i)
    {
        if (n % i == 0u)
        {
            sum += _a111336(i);
            idx_t j = n / i;
            if (j != i)
                sum += _a111336(j);
        }
    }
    return sum;
}

/*
 * A308097  Take the integer-sided triangles with perimeter n and integer
 *          area. Then a(n) is the sum of the areas of all the triangles and
 *          the squares on their sides.
 * https://oeis.org/A308097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum over all integer‑sided triangles with perimeter n and
 *            integer area of (area + a² + b² + c²). This implementation
 *            enumerates all candidate triangles, checks the Heron area
 *            condition with integer arithmetic, and accumulates the result
 * Range:     u8 n <= 13, u16 n <= 51, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Return true iff x is a perfect square. */
static bool is_square(seq_t x)
{
    seq_t r = floor_sqrt(x);
    return r * r == x;
}

seq_t a(idx_t n)
{
    /* No Heronian triangle has an odd perimeter. */
    if (n & 1) return 0;

    seq_t sum = 0;
    seq_t p = (seq_t)n / 2;               /* semiperimeter */
    idx_t max_a = n / 3;                  /* a ≤ n/3 for sorted sides */

    for (idx_t a = 1; a <= max_a; ++a) {
        idx_t max_b = (n - a) / 2;        /* ensures b ≤ c */
        for (idx_t b = a; b <= max_b; ++b) {
            idx_t c = n - a - b;
            if (a + b <= c) continue;    /* triangle inequality */

            seq_t f1 = p;
            seq_t f2 = p - a;
            seq_t f3 = p - b;
            seq_t f4 = p - c;

            /* area² = p(p-a)(p-b)(p-c) */
            seq_t area2 = f1 * f2;
            area2 = area2 * f3;
            area2 = area2 * f4;

            if (!is_square(area2)) continue;

            seq_t area = floor_sqrt(area2);

            seq_t sa = a, sb = b, sc = c;
            seq_t side_sq_sum = sa * sa + sb * sb + sc * sc;

            sum += area;
            sum += side_sq_sum;
        }
    }
    return sum;
}

/*
 * A397518  a(n) is the number of integer solutions of x^2 + 7*y^2 = 2*n + 1.
 * https://oeis.org/A397518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions of x^2 + 7*y^2 = 2*n + 1
 * Range:     u8 n <= 31, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count solutions (x,y) ∈ ℤ².  The number of solutions is always ≤ 2√( (2n+1)/7 ) + 1,
 * so this algorithm is easily fast for n up to 10,000. */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* M = 2*n + 1, cast to seq_t to avoid overflow when n is large */
    seq_t M = (seq_t)2 * (seq_t)n + 1;

    /* Find maximal |y| such that 7*y^2 ≤ M */
    int64_t maxy = 0;
    for (;;) {
        seq_t ysq = (seq_t)(maxy + 1) * (seq_t)(maxy + 1);
        seq_t t = (seq_t)7 * ysq;
        if (t > M) break;
        ++maxy;
    }

    seq_t count = 0;

    for (int64_t y = -(int64_t)maxy; y <= (int64_t)maxy; ++y) {
        /* abs_y^2 */
        seq_t abs_y = (y >= 0) ? (seq_t)y : (seq_t)(-y);
        seq_t ysq = abs_y * abs_y;

        seq_t t = (seq_t)7 * ysq;
        /* Since 7*ysq ≤ M by construction of maxy, subtraction is safe */
        seq_t r = M - t;

        seq_t k = floor_sqrt(r);
        if (k * k == r) {
            if (k == 0)
                ++count;    /* x = 0 gives one solution */
            else
                count += 2; /* x = ±k gives two solutions */
        }
    }

    return count;
}

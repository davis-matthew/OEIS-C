/*
 * A343897  Number of ways to write n as 2*x + y + z with x,y,z positive
 *          integers such that 16*x^2*y^2 + 19*y^2*z^2 + 29*z^2*x^2 is a
 *          square.
 * https://oeis.org/A343897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n as 2*x + y + z with * x, y, z
 *            positive integers such that * 16*x^2*y^2 + 19*y^2*z^2 +
 *            29*z^2*x^2 is a perfect square. * * The implementation
 *            enumerates all triples (x, y, z) satisfying * 2*x + y + z = n,
 *            all of which must be positive. For each * triple it evaluate...
 * Range:     u8 n <= 1500, u16 n <= 1500, u32 n <= 1500, u64 n <= 1500,
 *            u128 n <= 1500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

static inline uint64_t isqrt_u64(uint64_t x)
{
    /* Initial estimate from floating point sqrt.  A single
     * adjustment step is enough to guarantee correctness. */
    uint64_t r = sqrt((long double)x);
    while ((r + 1) * (r + 1) <= x) ++r;
    while (r * r > x) --r;
    return r;
}

seq_t a(idx_t n)
{
    uint64_t count = 0;
    uint64_t N = (uint64_t)n;

    /* x must be at least 1 and 2*x < n. */
    for (uint64_t x = 1; x <= (N - 1) / 2; ++x) {
        uint64_t rem = N - 2 * x;          /* y + z  (rem ≥ 2) */
        if (rem < 2) continue;            /* safety for n < 4 */
        for (uint64_t y = 1; y <= rem - 1; ++y) {
            uint64_t z = rem - y;

            uint64_t x2 = x * x;
            uint64_t y2 = y * y;
            uint64_t z2 = z * z;

            /* S = 16*x^2*y^2 + 19*y^2*z^2 + 29*z^2*x^2 */
            uint64_t S = 16ULL * x2 * y2
                       + 19ULL * y2 * z2
                       + 29ULL * z2 * x2;

            uint64_t t = isqrt_u64(S);
            if (t * t == S) ++count;
        }
    }
    return (seq_t)count;
}

/*
 * A393713  a(n) = number of triples (x, y, z) such that x^2 + y*z = n, where
 *          x, y, z are positive integers satisfying y < x < z.
 * https://oeis.org/A393713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (x, y, z) of positive integers such
 *            that y < x < z and x^2 + y*z = n
 * Range:     u8 n <= 434, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    /* iterate over possible x values */
    for (idx_t x = 1; ; ++x) {
        seq_t sq = (seq_t)x * x;      /* x^2 */
        if (sq >= (seq_t)n)            /* no larger x will satisfy x^2 < n */
            break;
        idx_t m = n - x*x;            /* product y*z = m > 0 */
        if (m == 0)                   /* cannot have y*z = 0 with positive y,z */
            continue;
        /* enumerate all divisor pairs (y, z) of m with y < z */
        for (idx_t y = 1; y*y <= m; ++y) {
            if (m % y != 0)
                continue;
            idx_t z = m / y;
            if (y >= z)               /* ensure y < z */
                continue;
            if (y < x && x < z)       /* ensure y < x < z */
                ++count;
        }
    }
    return count;
}

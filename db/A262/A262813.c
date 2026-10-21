/*
 * A262813  Number of ordered ways to write n as x^3 + y^2 + z*(z+1)/2 with x
 *          >= 0, y >=0 and z > 0.
 * https://oeis.org/A262813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (x,y,z) with * x >= 0, y >= 0,
 *            z > 0 such that n = x^3 + y^2 + z*(z+1)/2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root: floor(sqrt(v)) for unsigned 64‑bit v. */
static idx_t floor_isqrt(idx_t v)
{
    idx_t lo = 0;
    idx_t hi = v;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq == v) return mid;
        if (sq < v)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi; /* hi is floor(sqrt(v)) */
}

seq_t a(idx_t n)
{
    /* n is 1‑based by definition, but the algorithm works for any n>=1. */
    if (n == 0) return 0; /* undefined for n=0 – return 0 */

    seq_t count = 0;

    for (idx_t x = 0; ; ++x) {
        idx_t cube = x * x * x;
        if (cube > n) break;

        for (idx_t y = 0; ; ++y) {
            idx_t sq   = y * y;
            idx_t sum  = cube + sq;
            if (sum > n) break;

            idx_t rem = n - sum;          /* must equal z(z+1)/2 with z>0 */
            if (rem == 0) continue;       /* z>0 ⇒ triangular number >=1 */

            /* rem = z(z+1)/2  ⇔  8*rem + 1 = (2z+1)^2 */
            idx_t d = 1 + 8 * rem;
            idx_t sqd = floor_isqrt(d);
            if (sqd * sqd == d && (sqd & 1ULL) == 1ULL) {
                /* (sqd-1)/2 gives z >= 1 automatically */
                count += 1;
            }
        }
    }

    return count;
}

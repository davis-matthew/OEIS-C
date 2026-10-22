/*
 * A301626  Square array T(n, k) read by antidiagonals, n >= 0 and k >= 0:
 *          T(n, k) = square of the distance from n + k*i to nearest cube of a
 *          Gaussian integer (where i denotes the root of -1 with positive
 *          imaginary part).
 * https://oeis.org/A301626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   a(n) is the square of the Euclidean distance from the Gaussian
 *            integer n + k·i (read along antidiagonals) to the nearest cube
 *            of a Gaussian integer. N is 0‑based as in the OEIS entry
 * Range:     u8 n <= 2235, u16 n <= 8767, u32 n <= 8767, u64 n <= 8767,
 *            u128 n <= 8767, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline idx_t int_cube_root(idx_t n)
{
    idx_t lo = 0, hi = 0xFFFFFFFFULL, mid, ans = 0;
    while (lo <= hi) {
        mid = (lo + hi) / 2;
        unsigned __int128 cube = (unsigned __int128)mid * mid * mid;
        if (cube <= n) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Find antidiagonal coordinates (i, j) for index n. */
    idx_t lo = 0, hi = 0xFFFFFFFFULL, mid, d = 0;
    while (lo <= hi) {
        mid = (lo + hi) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n) {
            d = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    idx_t t = d * (d + 1) / 2;
    idx_t i = n - t;
    idx_t j = d - i;

    /* Determine a search radius for Gaussian integer cube coordinates. */
    idx_t max_coord = (i > j) ? i : j;
    idx_t radius = int_cube_root(max_coord) + 2;            /* conservative bound */

    /* Search all cubes (a + b·i)^3 within the radius. */
    int64_t minDist = -1;   /* -1 represents 'unset' */
    for (int64_t a = -(int64_t)radius; a <= (int64_t)radius; ++a) {
        for (int64_t b = -(int64_t)radius; b <= (int64_t)radius; ++b) {
            int64_t u = a * a * a - 3 * a * b * b;          /* real part */
            int64_t v = 3 * a * a * b - b * b * b;          /* imag part */
            int64_t du = (int64_t)i - u;
            int64_t dv = (int64_t)j - v;
            long long distSq = (long long)du * du + (long long)dv * dv;
            if (minDist == -1 || distSq < minDist) {
                minDist = distSq;
                if (minDist == 0) {          /* coincides with a cube */
                    return 0;
                }
            }
        }
    }
    return (seq_t)minDist;
}

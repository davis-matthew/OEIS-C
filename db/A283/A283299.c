/*
 * A283299  Number of ways to write 2*n + 1 as x^2 + 2*y^2 + 3*z^2 with x,y,z
 *          integers such that x + y + z is a square or twice a square.
 * https://oeis.org/A283299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer triples (x,y,z) with 2*n+1 =
 *            x^2+2*y^2+3*z^2 * and s = x + y + z a non‑negative square or
 *            twice a square
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_square_or_twice_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_square_or_twice_square.h"

/* integer square root for unsigned 64‑bit values (floor).  The values are tiny. */
static inline uint64_t int_sqrt_u64(uint64_t n)
{
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= n)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    uint64_t M = 2 * n + 1;                /* 2*n + 1 as 64‑bit integer */
    int maxz = (int)int_sqrt_u64(M / 3);   /* |z| <= sqrt(M/3) */
    int maxy = (int)int_sqrt_u64(M / 2);   /* |y| <= sqrt(M/2) */
    seq_t res = 0;

    for (int z = -maxz; z <= maxz; ++z) {
        uint64_t z2 = 3ULL * (uint64_t)z * (uint64_t)z;
        if (z2 > M)
            continue;                       /* should not happen, safety */
        uint64_t rem_y = M - z2;
        for (int y = -maxy; y <= maxy; ++y) {
            uint64_t y2 = 2ULL * (uint64_t)y * (uint64_t)y;
            if (y2 > rem_y)
                continue;
            uint64_t rem = rem_y - y2;      /* x^2          */
            /* check if rem is a perfect square */
            int t = (int)int_sqrt_u64(rem);
            if ((uint64_t)t * (uint64_t)t != rem)
                continue;

            /* x = +t                 */
            int s = t + y + z;
            if (s >= 0 && is_square_or_twice_square(s))
                ++res;

            if (t != 0) {                  /* x = -t   (distinct if t != 0) */
                int s2 = -t + y + z;
                if (s2 >= 0 && is_square_or_twice_square(s2))
                    ++res;
            }
        }
    }
    return res;
}

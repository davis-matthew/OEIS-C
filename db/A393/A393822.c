/*
 * A393822  a(n) is the number of unit cubes in the cubic lattice, divided by
 *          24, that have 2 vertices strictly inside and 6 vertices strictly
 *          outside the sphere x^2 + y^2 + z^2 = n.
 * https://oeis.org/A393822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A393822: a(n) = number of unit cubes in the integer lattice
   (up to rotation) that have exactly 2 vertices strictly inside
   the sphere x^2 + y^2 + z^2 = n and 6 vertices strictly outside.
   The function returns that number divided by 24, as required by the
   sequence definition. */
#include <stdint.h>
#include <stdbool.h>

/* floor(sqrt(n)) for 64‑bit unsigned integers */
static inline uint64_t floor_isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = (n < 0xFFFFFFFFULL ? n : 0xFFFFFFFFULL);
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        uint64_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    uint64_t nn = n;                 /* radius squared */
    uint64_t m   = floor_isqrt_u64(nn);

    /* bounds for the corner of a unit cube that can have an inside vertex */
    int64_t min_coord = -(int64_t)m - 1;   /* lower corner coordinate */
    int64_t max_coord =  (int64_t)m;       /* upper corner coordinate */

    seq_t tot = 0;                     /* total cubes before division */

    for (int64_t x = min_coord; x <= max_coord; ++x) {
        for (int64_t y = min_coord; y <= max_coord; ++y) {
            for (int64_t z = min_coord; z <= max_coord; ++z) {
                int inside  = 0;
                int outside = 0;
                bool bad = false;

                for (int dx = 0; dx <= 1; ++dx) {
                    int64_t cx = x + dx;
                    for (int dy = 0; dy <= 1; ++dy) {
                        int64_t cy = y + dy;
                        for (int dz = 0; dz <= 1; ++dz) {
                            int64_t cz = z + dz;
                            int64_t dist = cx*cx + cy*cy + cz*cz;
                            if (dist == (int64_t)nn) {
                                bad = true;
                                goto next_cube;
                            } else if (dist < (int64_t)nn) {
                                ++inside;
                            } else {
                                ++outside;
                            }
                        }
                    }
                }

                if (!bad && inside == 2 && outside == 6) {
                    ++tot;
                }

            next_cube:
                /* continue with next cube */
                ;
            }
        }
    }

    return tot / 24;   /* divide by 24 as specified */
}

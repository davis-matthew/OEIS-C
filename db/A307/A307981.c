/*
 * A307981  Number of ways to write n as x^3 + 2*y^3 + 3*z^3 +
 *          w*(w+1)*(w+2)/6, where x,y,z,w are nonnegative integers.
 * https://oeis.org/A307981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A307981
 *  a(n) = number of ways to write n as x^3 + 2*y^3 + 3*z^3 + w*(w+1)*(w+2)/6
 *  where x,y,z,w are nonnegative integers.
 */
#include <stdint.h>
#include <stdbool.h>

/* Helper: compute the floor of the cube root of a 64‑bit integer. */
static uint64_t floor_cuberoot(uint64_t n)
{
    if (n < 8) {
        uint64_t r = 0;
        while ((unsigned __int128)(r + 1)*(r + 1)*(r + 1) <= n) r++;
        return r;
    }
    uint64_t low = 0, high = 1;
    /* grow high until high^3 > n */
    while ((unsigned __int128)high*high*high <= n) high <<= 1;
    /* binary search for the largest low such that low^3 <= n */
    while (high > low + 1) {
        uint64_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid*mid*mid <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/*  External interface required by the build system */
seq_t a(idx_t n)
{
    seq_t count = 0;          /* result accumulator */
    uint64_t n64 = (uint64_t)n;

    for (uint64_t w = 0; ; ++w) {
        unsigned __int128 wval = (unsigned __int128)w * (w + 1) * (w + 2) / 6;
        if (wval > n64) break;                 /* no more w possible */

        uint64_t rest_w = n64 - (uint64_t)wval; /* remaining after w contribution */

        for (uint64_t z = 0; ; ++z) {
            unsigned __int128 val_z = (unsigned __int128)3 * z * z * z;
            if (val_z > rest_w) break;          /* too large z */

            uint64_t rest_z = rest_w - (uint64_t)val_z;

            for (uint64_t y = 0; ; ++y) {
                unsigned __int128 val_y = (unsigned __int128)2 * y * y * y;
                if (val_y > rest_z) break;      /* too large y */

                uint64_t rest_y = rest_z - (uint64_t)val_y;

                uint64_t x = floor_cuberoot(rest_y);
                if ((unsigned __int128)x * x * x == rest_y)
                    count++;                    /* valid quadruple found */
            }
        }
    }

    return count;
}

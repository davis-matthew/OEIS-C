/*
 * A287616  Number of ways to write n as x*(x+1)/2 + y*(3*y+1)/2 + z*(5*z+1)/2
 *          with x,y,z nonnegative integers.
 * https://oeis.org/A287616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of non‑negative integer solutions (x,y,z) of * n =
 *            x*(x+1)/2 + y*(3*y+1)/2 + z*(5*z+1)/2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square root for 64‑bit values: floor(sqrt(n)) */
static uint64_t isqrt_u64(uint64_t n) {
    uint64_t res = 0;
    uint64_t bit = 1ULL << 62;          /* largest power of four <= n */
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n) {
    uint64_t n64 = (uint64_t)n;

    /* compute upper bounds for x, y, z */
    uint64_t xmax = (isqrt_u64(1 + 8 * n64) - 1) / 2;                /* from x(x+1)/2 <= n */
    uint64_t ymax = (isqrt_u64(1 + 24 * n64) - 1) / 6;                /* from y(3y+1)/2 <= n */
    uint64_t zmax = (isqrt_u64(1 + 40 * n64) - 1) / 10;               /* from z(5z+1)/2 <= n */

    seq_t count = 0;

    for (uint64_t x = 0; x <= xmax; ++x) {
        uint64_t val_x = x * (x + 1) / 2;
        for (uint64_t y = 0; y <= ymax; ++y) {
            uint64_t val_y = y * (3 * y + 1) / 2;
            uint64_t sum_xy = val_x + val_y;
            if (sum_xy > n64) continue;

            uint64_t r = n64 - sum_xy;                          /* remaining part for z */
            uint64_t D = 1 + 40 * r;                           /* discriminant 1+40r */
            uint64_t sqrtD = isqrt_u64(D);
            if (sqrtD * sqrtD != D) continue;                  /* sqrtD not integer */
            if (sqrtD < 1) continue;
            if ((sqrtD - 1) % 10 != 0) continue;                /* z must be integral */

            uint64_t z = (sqrtD - 1) / 10;
            if (z <= zmax) ++count;                           /* valid triple found */
        }
    }
    return count;
}

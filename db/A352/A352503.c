/*
 * A352503  Number of ways to write n as w^3 + 2*x^3 + 4*y^3 + 5*z^3 + t^6,
 *          where w is a positive integer, and x,y,z,t are nonnegative
 *          integers.
 * https://oeis.org/A352503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions of n = w³ + 2·x³ + 4·y³ +
 *            5·z³ + t⁶ with w ≥ 1, x,y,z,t ≥ 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/int_cube_root.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    uint64_t N = (uint64_t)n;
    uint64_t count = 0;

    for (uint64_t t = 0; ; ++t) {
        uint64_t t6 = t * t * t * t * t * t;        /* t⁶ */
        if (t6 > N) break;
        uint64_t rem1 = N - t6;                    /* after t⁶ */

        uint64_t wmax = int_cube_root(rem1);
        for (uint64_t w = 1; w <= wmax; ++w) {
            uint64_t w3 = w * w * w;                /* w³ */
            uint64_t rem2 = rem1 - w3;              /* after w³ */

            uint64_t xmax = int_cube_root(rem2 / 2);
            for (uint64_t x = 0; x <= xmax; ++x) {
                uint64_t x3_2 = 2 * x * x * x;      /* 2·x³ */
                uint64_t rem3 = rem2 - x3_2;        /* after 2·x³ */

                uint64_t ymax = int_cube_root(rem3 / 4);
                for (uint64_t y = 0; y <= ymax; ++y) {
                    uint64_t y3_4 = 4 * y * y * y;  /* 4·y³ */
                    uint64_t rem4 = rem3 - y3_4;    /* after 4·y³ */

                    if (rem4 % 5 != 0) continue;    /* must be a multiple of 5 */
                    uint64_t z3 = rem4 / 5;
                    uint64_t z = int_cube_root(z3);
                    if (z * z * z == z3) ++count;    /* found a valid (w,x,y,z,t) */
                }
            }
        }
    }

    return (seq_t)count;
}

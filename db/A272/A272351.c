/*
 * A272351  Number of ordered ways to write n as w^2 + x^2 + y^2 + z^2 with
 *          x^4 + 8*y*z*(y^2+z^2) a fourth power, where w,x,y,z are
 *          nonnegative integers with w >= x and y > z.
 * https://oeis.org/A272351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A272351
   Number of ordered ways to write n as w² + x² + y² + z²
   with w ≥ x, y > z,  and  x⁴ + 8·y·z·(y²+z²) a fourth power.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* integer square‑root, floor of √x for 0 ≤ x < 2⁶⁴          */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t res = 0;
    uint64_t bit = 1ULL << 62;   /* highest power of four ≤ 2⁶⁴                */
    while (bit > n) bit >>= 2;
    while (bit != 0) {
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

/* test whether v is a perfect fourth power                 */
static bool is_fourth_power(uint64_t v)
{
    uint64_t s = isqrt_u64(v);
    if (s * s != v) return false;
    uint64_t t = isqrt_u64(s);
    return t * t == s;
}

/* main sequence function                                           */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* out of offset – defined as 0 */

    uint64_t N = (uint64_t)n;
    uint32_t sqrtN = (uint32_t)isqrt_u64(N);

    /* pre‑compute x⁴ for all x ≤ sqrtN                          */
    uint64_t *x4 = (uint64_t*)malloc((size_t)(sqrtN + 1) * sizeof(uint64_t));
    if (!x4) return (seq_t)0;               /* allocation failure      */
    for (uint32_t x = 0; x <= sqrtN; ++x) {
        uint64_t x2 = (uint64_t)x * (uint64_t)x;
        x4[x] = x2 * x2;                    /* x⁴                                                        */
    }

    uint64_t count = 0;

    for (uint32_t w = 0; w <= sqrtN; ++w) {
        uint64_t w2 = (uint64_t)w * (uint64_t)w;
        if (w2 > N) continue;

        for (uint32_t x = 0; x <= w; ++x) {
            uint64_t x2 = (uint64_t)x * (uint64_t)x;
            uint64_t sum_wx = w2 + x2;
            if (sum_wx > N) continue;

            uint64_t rem = N - sum_wx;           /* required value of y² + z²     */
            if (rem == 0) continue;              /* y²+z² cannot be 0 with y>z   */

            uint32_t sqrtRem = (uint32_t)isqrt_u64(rem);

            for (uint32_t y = 1; y <= sqrtRem; ++y) {
                uint64_t y2 = (uint64_t)y * (uint64_t)y;
                if (y2 > rem) break;

                uint64_t z2 = rem - y2;
                if (z2 >= y2) continue;          /* ensure z < y  (z² < y²)     */

                uint64_t z = isqrt_u64(z2);
                if (z * z != z2) continue;       /* z² must be perfect square   */

                /* now (y,z) satisfies y > z, y²+z² = rem    */
                unsigned __int128 tmp = (unsigned __int128)x4[x]
                                         + (unsigned __int128)8 * y * z * rem;
                uint64_t val = (uint64_t)tmp;
                if (is_fourth_power(val)) ++count;
            }
        }
    }

    free(x4);
    return (seq_t)count;
}

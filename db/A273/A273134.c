/*
 * A273134  Number of ordered ways to write n as x^2 + y^2 + z^2 + w^2 with
 *          (x+8*y+8*z+15*w)^2+(6*(x+y+z+w))^2 a square, where x,y,z,w are
 *          nonnegative integers with y < z.
 * https://oeis.org/A273134
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

/*
 * Formula: a(n) = number of ordered quadruples (x,y,z,w) of
 * non‑negative integers with y < z such that
 *          x² + y² + z² + w² = n
 * and (x + 8y + 8z + 15w)² + (6·(x + y + z + w))²
 * is a perfect square.
 *
 * The function computes this by enumerating all quadruples
 * whose sum of squares does not exceed the requested n.
 * The computation is memoised – only the first call with
 * a requested value larger than all previous ones triggers
 * a recomputation for the new maximum.
 */

#include <stdlib.h>
#include <stdint.h>

/* integer square root of a 64‑bit unsigned integer */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t r = 0;
    uint64_t bit = 1ULL << 62;            /* the largest even power of two <= x */
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* static cache of computed values */
static seq_t *cache = NULL;
static idx_t cache_max = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* Undefined for n < 1, return 0 */

    /* Recompute cache if needed */
    if (!cache || n > cache_max) {
        /* free old cache if any */
        free(cache);

        /* allocate new cache and initialise to zero */
        size_t sz = (size_t)(n + 1);
        cache = (seq_t *)calloc(sz, sizeof(seq_t));
        if (!cache) return 0;   /* allocation failure – return 0 */

        /* enumerate all admissible quadruples up to the new maximum */
        uint64_t max_w = isqrt_u64(n);
        for (uint64_t w = 0; w <= max_w; ++w) {
            uint64_t w2 = w * w;
            if (w2 > n) break;

            uint64_t rem_w = n - w2;
            uint64_t max_z = isqrt_u64(rem_w);
            for (uint64_t z = 0; z <= max_z; ++z) {
                uint64_t z2 = z * z;
                uint64_t wz2 = w2 + z2;
                if (wz2 > n) break;

                uint64_t rem_wz = n - wz2;
                uint64_t max_y = isqrt_u64(rem_wz);
                for (uint64_t y = 0; y < z; ++y) {   /* enforce y < z */
                    uint64_t y2 = y * y;
                    uint64_t wyz = wz2 + y2;
                    if (wyz > n) break;

                    uint64_t rem = n - wyz;
                    uint64_t max_x = isqrt_u64(rem);
                    for (uint64_t x = 0; x <= max_x; ++x) {
                        uint64_t sumsq = wyz + x * x;  /* current value of n0 */

                        /* Pythagorean condition */
                        uint64_t k = x + 8 * y + 8 * z + 15 * w;
                        uint64_t m = 6 * (x + y + z + w);
                        uint64_t sq = k * k + m * m;
                        uint64_t t = isqrt_u64(sq);
                        if (t * t == sq) {
                            cache[(size_t)sumsq]++;   /* count this representation */
                        }
                    }
                }
            }
        }

        cache_max = n;
    }

    return cache[(size_t)n];
}

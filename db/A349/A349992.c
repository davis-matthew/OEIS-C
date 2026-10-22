/*
 * A349992  Number of ways to write n as x^4 + y^2 + (z^2 + 2*4^w)/3, where x,
 *          y, z are nonnegative integers, and w is 0 or 1.
 * https://oeis.org/A349992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of ways to write n as x^4 + y^2 + (z^2 + 2*4^w)/3 where
 *            x, y, z are nonnegative integers and w ∈ {0,1}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for 64‑bit unsigned values (floor of √n). */
static idx_t isqrt_idx(idx_t n)
{
    idx_t r = 0;
    idx_t bit = (idx_t)1 << 62;   /* highest power of 2 ≤ 2^64 */

    while (bit != 0) {
        idx_t nxt = r | bit;
        if ((__int128)nxt * nxt <= n)
            r = nxt;
        bit >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety: OEIS offset starts at 1 */

    idx_t N   = n;
    seq_t cnt = 0;

    for (idx_t x = 0; ; ++x) {
        /* x^4 = (x^2)^2 ; use 128‑bit for safety */
        __int128 x2_128 = (__int128)x * x;
        __int128 x4_128 = x2_128 * x2_128;
        if (x4_128 > N) break;

        idx_t R = N - (idx_t)x4_128;   /* remainder after x^4 */

        /* w = 0  →  (z^2 + 2)/3  */
        const idx_t c0 = 2;
        if (R >= 1) {
            __int128 max_z_sqr_full = (__int128)3 * R - c0;
            if (max_z_sqr_full >= 1) {
                idx_t limitZ = isqrt_idx((idx_t)max_z_sqr_full);
                for (idx_t z = 1; z <= limitZ; ++z) {
                    if (z % 3 == 0) continue;   /* ensures divisibility by 3 */
                    idx_t z2 = (idx_t)((__int128)z * z);
                    idx_t t  = (idx_t)(((__int128)z2 + c0) / 3);
                    idx_t rem = R - t;
                    idx_t r  = isqrt_idx(rem);
                    if ((__int128)r * r == rem) ++cnt;
                }
            }
        }

        /* w = 1  →  (z^2 + 8)/3  */
        const idx_t c1 = 8;
        if (R >= 3) {
            __int128 max_z_sqr_full = (__int128)3 * R - c1;
            if (max_z_sqr_full >= 1) {
                idx_t limitZ = isqrt_idx((idx_t)max_z_sqr_full);
                for (idx_t z = 1; z <= limitZ; ++z) {
                    if (z % 3 == 0) continue;
                    idx_t z2 = (idx_t)((__int128)z * z);
                    idx_t t  = (idx_t)(((__int128)z2 + c1) / 3);
                    idx_t rem = R - t;
                    idx_t r  = isqrt_idx(rem);
                    if ((__int128)r * r == rem) ++cnt;
                }
            }
        }
    }

    return cnt;
}

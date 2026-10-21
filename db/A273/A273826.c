/*
 * A273826  Number of ordered ways to write n as x^2 + y^2 + z^2 + w^2 with
 *          x*y + y*z + z*w a fourth power, where x is a positive integer, y
 *          is a nonnegative integer, and z and w are integers.
 * https://oeis.org/A273826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7000, u16 n <= 7000, u32 n <= 7000, u64 n <= 7000,
 *            u128 n <= 7000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A273826
   a(n) = number of ordered quadruples (x,y,z,w) such that
          x^2 + y^2 + z^2 + w^2 = n,
          x > 0, y ≥ 0, z,w integers,
          and x*y + y*z + z*w is a perfect fourth power.
   The function below enumerates all candidate quadruples
   with careful use of 64‑bit arithmetic to avoid overflow on
   any supported seq_t size. */

#include <stdint.h>
#include <stdbool.h>

/* Integer square‑root for an unsigned 64‑bit value.
   Returns ⌊√n⌋. */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = n;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Check whether u is a perfect fourth power. */
static inline bool is_fourth_power(uint64_t u)
{
    uint64_t r  = isqrt_u64(u);   /* r = ⌊√u⌋   */
    uint64_t r2 = isqrt_u64(r);   /* r2 = ⌊√r⌋  */
    return (r2 * r2 * r2 * r2) == u;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    unsigned long long cnt = 0;          /* use 64‑bit counter to avoid overflow */

    /* pre‑compute maximum x and y. */
    idx_t xmax = isqrt_u64(n);           /* floor(sqrt(n)) */
    for (idx_t x = 1; x <= xmax; ++x) {
        uint64_t x2 = (uint64_t)x * (uint64_t)x;
        if (x2 > n) break;
        uint64_t rem_after_x = n - x2;   /* y^2 + z^2 + w^2 */

        idx_t ymax = isqrt_u64(rem_after_x);
        for (idx_t y = 0; y <= ymax; ++y) {
            uint64_t y2 = (uint64_t)y * (uint64_t)y;
            uint64_t rem = rem_after_x - y2;   /* z^2 + w^2 */

            /* Handle the special case z = w = 0 */
            if (rem == 0) {
                long long s = (long long)x * (long long)y; /* x*y + y*0 + 0*0 */
                if (s >= 0 && is_fourth_power((uint64_t)s))
                    ++cnt;
                continue;
            }

            idx_t zmax = isqrt_u64(rem);
            /* Iterate over all integer z (positive, negative, and zero) */
            for (int z = -(int)zmax; z <= (int)zmax; ++z) {
                uint64_t z2 = (uint64_t)z * (uint64_t)z;
                uint64_t w2 = rem - z2;
                if (w2 > rem) continue;  /* safety guard, should never happen */

                /* w is integer iff sqrt(w2) is integer */
                uint64_t wsq = isqrt_u64(w2);
                if (wsq * wsq != w2)
                    continue;

                /* Iterate over the two possible signs of w (or one if zero) */
                int sign_w_start = (wsq == 0) ? 0 : -1;
                for (int sign = sign_w_start; sign <= 1; sign += (wsq == 0 ? 2 : 2)) {
                    int w = (int)(sign * (int)wsq);
                    long long s = (long long)x * (long long)y
                                  + (long long)y * (long long)z
                                  + (long long)z * (long long)w;
                    if (s >= 0 && is_fourth_power((uint64_t)s))
                        ++cnt;
                }
            }
        }
    }

    return (seq_t)cnt;
}

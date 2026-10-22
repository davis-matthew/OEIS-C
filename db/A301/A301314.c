/*
 * A301314  Number of ways to write n as x^2 + y^2 + z^2 + w^2, where w is a
 *          positive integer and x,y,z are nonnegative integers such that x +
 *          3*y + 9*z = 2^k*m^3 for some k,m = 0,1,2,....
 * https://oeis.org/A301314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of quadruples (x,y,z,w) with w>0, x,y,z≥0, x² + y²
 *            + z² + w² = n, and x + 3y + 9z = 2^k * m³ for some integers
 *            k,m≥0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force enumeration of all valid representations. */

#include <stdint.h>
#include <math.h>

/* Integer square root for unsigned 64‑bit values. */
static inline idx_t isqrt_uint64(idx_t v)
{
    /* Initial approximation with double. */
    double d = sqrt((double)v);
    idx_t r = (idx_t)d;
    while ((unsigned __int128)(r + 1) * (r + 1) <= v) ++r;
    while ((unsigned __int128)r * r > v) --r;
    return r;
}

/* Integer cube root for unsigned 64‑bit values. */
static inline idx_t icbrt_uint64(idx_t v)
{
    if (v < 8) return v;          /* 0³–7³ all give r = v */
    idx_t r = (idx_t)pow((double)v, 1.0 / 3.0);
    /* Adjust up or down to get the floor of the real cube root. */
    while ((unsigned __int128)(r + 1) * (r + 1) * (r + 1) <= v) ++r;
    while ((unsigned __int128)r * r * r > v) --r;
    return r;
}

/* Count solutions for a given n. */
seq_t a(idx_t n)
{
    seq_t result = 0;

    if (n == 0) return 0;         /* not used, but harmless */

    /* w must be positive and w² ≤ n */
    idx_t max_w = isqrt_uint64(n);

    for (idx_t w = 1; w <= max_w; ++w) {
        idx_t w2 = w * w;
        idx_t rem_w = n - w2;                  /* remaining after w² */

        idx_t max_z = isqrt_uint64(rem_w);
        for (idx_t z = 0; z <= max_z; ++z) {
            idx_t z2 = z * z;
            idx_t rem_z = rem_w - z2;          /* remaining after z² */

            idx_t max_y = isqrt_uint64(rem_z);
            for (idx_t y = 0; y <= max_y; ++y) {
                idx_t y2 = y * y;
                idx_t rem_y = rem_z - y2;      /* remaining after y² */

                idx_t x = isqrt_uint64(rem_y);
                if (x * x != rem_y) continue;  /* x must be integral */

                /* Check the special form of x + 3*y + 9*z. */
                idx_t u = x + 3 * y + 9 * z;
                if (u == 0) {                 /* trivial case m = 0 */
                    ++result;
                    continue;
                }

                /* Remove all factors of 2: u = 2^k * odd. */
                idx_t t = __builtin_ctzll((unsigned long long)u);
                idx_t odd = u >> t;

                /* odd must be a perfect cube. */
                idx_t c = icbrt_uint64(odd);
                if ((unsigned __int128)c * c * c == odd) {
                    ++result;
                }
            }
        }
    }

    return result;
}

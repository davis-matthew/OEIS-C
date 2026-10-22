/*
 * A300792  Number of ways to write n as x^2 + y^2 + z^2 + w^2, where w is a
 *          positive integer and x,y,z are nonnegative integers such that x or
 *          y or z is a square and 9*x^2 + 16*y^2 + 24*z^2 is also a square.
 * https://oeis.org/A300792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n = x² + y² + z² + w² * where w >
 *            0, x,y,z ≥ 0, at least one of x,y,z is a square, and * 9*x² +
 *            16*y² + 24*z² is a perfect square. * Computation is by
 *            exhaustive search over all (x,y,z) with x² + y² + z² < n * and
 *            counting only tuples that satisfy the constraints
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Integer square root of a 64‑bit unsigned integer using Newton's method. */
static inline idx_t isqrt_u64(idx_t n)
{
    if (n < 2)
        return n;
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* Return true if v is a perfect square (based on isqrt_u64). */
static inline bool is_square_uint64(idx_t v)
{
    idx_t r = isqrt_u64(v);
    return r * r == v;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n == 0)
        return 0;                     /* defined for n >= 1 only */

    idx_t lim = n - 1;                 /* w² = n - (x²+y²+z²) ≥ 1 */

    /* Pre‑compute square flags for values up to sqrt(n) to avoid repeated sqrt. */
    idx_t root_lim = isqrt_u64(lim);
    bool is_sq_val[256] = {false};    /* 256 > sqrt(10000) */
    for (idx_t i = 0; i <= root_lim; ++i)
        is_sq_val[i * i] = true;

    for (idx_t x = 0; x * x <= lim; ++x) {
        idx_t xsq = x * x;
        for (idx_t y = 0; xsq + y * y <= lim; ++y) {
            idx_t ysq = y * y;
            idx_t partial = xsq + ysq;
            for (idx_t z = 0; partial + z * z <= lim; ++z) {
                idx_t zsq = z * z;
                idx_t sum = partial + zsq;

                idx_t w2 = n - sum;
                if (w2 == 0)           /* w must be positive */
                    continue;
                idx_t wroot = isqrt_u64(w2);
                if (wroot * wroot != w2)
                    continue;          /* w² must be a square */

                /* Test the quadratic form condition */
                idx_t t = 9 * xsq + 16 * ysq + 24 * zsq;
                idx_t trest = isqrt_u64(t);
                if (trest * trest != t)
                    continue;

                /* At least one of x, y, z must be a square */
                if (!is_sq_val[x] && !is_sq_val[y] && !is_sq_val[z])
                    continue;

                ++count;
            }
        }
    }
    return count;
}

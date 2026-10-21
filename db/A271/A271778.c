/*
 * A271778  Number of ordered ways to write n as x^2 + y^2 + z^2 + w^2 with
 *          x,y,z,w nonnegative integers and x^2 + 3*y^2 + 5*z^2 - 8*w^2 a
 *          square.
 * https://oeis.org/A271778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A271778
 *
 *  a(n) = number of ordered 4‑tuples (x,y,z,w) of non‑negative integers
 *         such that
 *               x² + y² + z² + w² = n
 *         and
 *               x² + 3·y² + 5·z² – 8·w²
 *         is a perfect square.
 *
 *  The implementation enumerates all possible values of w, x, y and
 *  checks whether the remaining value is a square z² and whether the
 *  quadratic expression is a square.  An integer square root routine
 *  operating on idx_t is implemented locally (no external helper needed).
 */

#include <stdint.h>

/* ----------------------------------------------------------------------
 *  Integer square root for unsigned 64‑bit integers.
 *  Returns floor(sqrt(n)).  Uses the Newton iteration.
 * ---------------------------------------------------------------------- */
static inline idx_t isqrt_u64(idx_t n)
{
    if (n == 0) return 0;
    idx_t x = n;
    idx_t y = (x + 1) >> 1;               /* (x+1)/2 */
    while (y < x) {
        x = y;
        /* n/x fits in idx_t and division by zero is impossible because x>0 */
        y = (x + n / x) >> 1;             /* (x + n/x)/2 */
    }
    return x;
}

/* ----------------------------------------------------------------------
 *  Main sequence function.
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    seq_t count = 0;
    const idx_t max_w = isqrt_u64(n);      /* upper bound for w */

    for (idx_t w = 0; w <= max_w; ++w) {
        const idx_t w2 = w * w;
        if (w2 > n) break;                /* safety, but not needed */

        const idx_t n1 = n - w2;
        const idx_t max_x = isqrt_u64(n1);

        for (idx_t x = 0; x <= max_x; ++x) {
            const idx_t x2 = x * x;
            const idx_t n2 = n1 - x2;
            if (n2 > n1) break;           /* underflow protection */

            const idx_t max_y = isqrt_u64(n2);

            for (idx_t y = 0; y <= max_y; ++y) {
                const idx_t y2 = y * y;
                const idx_t n3 = n2 - y2;
                if (n3 > n2) break;       /* underflow protection */

                /* n3 must be a perfect square for some integer z */
                const idx_t z = isqrt_u64(n3);
                if (z * z != n3) continue;/* not a square, skip */

                /* Compute the quadratic expression as a signed value
                 * (the subtraction can make it negative).  All intermediate
                 * numbers are far below the limits of int64_t for n ≤ 10000. */
                const int64_t expr =
                    (int64_t)x2
                  + 3LL * (int64_t)y2
                  + 5LL * (int64_t)n3   /* n3 == z² */
                  - 8LL * (int64_t)w2;

                if (expr < 0) continue;   /* cannot be a square */

                const idx_t u = (idx_t)expr;   /* convert to unsigned for sqrt */
                const idx_t t = isqrt_u64(u);
                if (t * t == u) {
                    ++count;                /* found a valid quadruple */
                }
            }
        }
    }

    return count;
}

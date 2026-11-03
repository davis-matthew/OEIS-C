/*
 * A362496  Square array A(n, k), n, k >= 0, read by upwards antidiagonals; if
 *          Newton's method applied to the complex function f(z) = z^3 - 1 and
 *          starting from n + k*i reaches or converges to exp(2*r*i*Pi/3) for
 *          some r in 0..2, then A(n, k) = r, otherwise A(n, k) = -1 (where i
 *          denotes the imaginary unit).
 * https://oeis.org/A362496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 90 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes OEIS A362496: a(n) is the row obtained by reading the
 * square array A(n,k) (Newton fractal) along upwards antidiagonals.
 * For index n, the antidiagonal coordinates (i,j) are obtained with
 * antidiagonal_coords.  The complex initial point is z = j + i·i
 * (swap of the coordinates) and Newton's method is applied to
 * f(z)=z³−1.  The returned value is a root index 0,1,2 or −1 if
 * the iteration does not converge to a root.
 */
#include <stdbool.h>
#include <math.h>

/* mapping from linear index n to pair (i,j) on the antidiagonals of T */
static inline void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    /* find s such that (s+1)(s+2)/2 <= n < (s+2)(s+3)/2 */
    idx_t s = 0;
    while (((s + 1) * (s + 2)) / 2 <= n) ++s;
    idx_t pos = n - (s * (s + 1)) / 2;  /* 0 <= pos <= s */
    *i = pos;
    *j = s - pos;
}

/* Performs Newton iteration for f(z)=z³−1 from the integer point
 * represented by (i , j).  The point is treated as (j + i·i)
 * in order to match the OEIS antidiagonal ordering.  The result
 * is 0,1,2 for the three cube‑root‑unity values or −1 for failure. */
static seq_t newton_root(idx_t i, idx_t j)
{
    /*  Start from the swapped coordinates: z = j + i·i */
    double x = (double)j;   /* real part */
    double y = (double)i;   /* imaginary part */

    /* Special case: at the origin the derivative vanishes */
    if (i == 0 && j == 0)
        return (seq_t)-1;

    const int       max_iter = 120;
    const double    eps      = 1e-14;

    const double sqrt3   = sqrt(3.0);
    const double r0x =  1.0, r0y =  0.0;
    const double r1x = -0.5, r1y =  sqrt3 / 2.0;
    const double r2x = -0.5, r2y = -sqrt3 / 2.0;

    for (int iter = 0; iter < max_iter; ++iter) {
        /* Compute z² = (x² – y²) + i·(2xy) */
        double z2x = x*x - y*y;
        double z2y = 2.0*x*y;

        /* |z²|² = (x² – y²)² + (2xy)² */
        double denom = z2x*z2x + z2y*z2y;
        if (denom == 0.0)
            return (seq_t)-1;       /* singular point */

        /* 1 / z² = (x² – y² – i·2xy) / |z²|² */
        double inv_x =  z2x / denom;
        double inv_y = -z2y / denom;

        /* Newton step: z_next = (2z + 1/z²)/3 */
        double nx = (2.0*x + inv_x) / 3.0;
        double ny = (2.0*y + inv_y) / 3.0;

        if (fabs(nx - x) < eps && fabs(ny - y) < eps) {
            x = nx; y = ny;
            break;          /* converged */
        }
        x = nx; y = ny;
    }

    /* Near‑root classification by Euclidean distance */
    double d0 = (x - r0x)*(x - r0x) + (y - r0y)*(y - r0y);
    double d1 = (x - r1x)*(x - r1x) + (y - r1y)*(y - r1y);
    double d2 = (x - r2x)*(x - r2x) + (y - r2y)*(y - r2y);

    double best = d0;
    seq_t  r    = 0;
    if (d1 < best) { best = d1; r = 1; }
    if (d2 < best) { best = d2; r = 2; }

    return r;
}

seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);
    /* use swapped coordinates to match the OEIS convention */
    return newton_root(i, j);
}

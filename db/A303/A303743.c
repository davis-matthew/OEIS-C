/*
 * A303743  a(n) is a number of lattice points in 3D Cartesian grid between
 *          cube with edge length 2*n centered in origin and its inscribed
 *          sphere. Three pairs of the cube's faces are parallel to the planes
 *          XOY, XOZ, YOZ respectively.
 * https://oeis.org/A303743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 26, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = (2*n-1)^3  –  (# of integer triples 0≤x,y,z≤n-1 with x²+y²+z²≤n²)  */
/*  The cube has side length 2*n centered at the origin.  Lattice points are taken
    strictly inside the cube (|x|<n, |y|<n, |z|<n).  The sphere inscribed in
    the cube has radius n.  a(n) counts the points inside the cube but
    outside the sphere.  */
#include <stdint.h>
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    idx_t n_minus_1 = n - 1;
    /* total number of interior lattice points of the cube */
    seq_t t = (seq_t)(2 * n_minus_1 + 1); // 2*n-1
    seq_t total = t * t * t;

    seq_t inside = 0;
    idx_t n_sq = n * n;

    for (idx_t x = 0; x <= n_minus_1; ++x)
    {
        idx_t ox = (x == 0) ? 1 : 2;                 /* sign factor for x */
        idx_t xsq = x * x;
        for (idx_t y = 0; y <= n_minus_1; ++y)
        {
            idx_t oy = (y == 0) ? 1 : 2;             /* sign factor for y */
            idx_t ysq = y * y;
            idx_t sum_xy = xsq + ysq;
            if (sum_xy > n_sq)                      /* no z can satisfy */
                continue;

            idx_t rem = n_sq - sum_xy;              /* remaining for z² */
            idx_t zmax = _floor_isqrt_u64(rem);     /* largest |z| satisfying z² ≤ rem */
            if (zmax > n_minus_1)
                zmax = n_minus_1;
            idx_t zcount = zmax + 1;                /* number of non‑negative z values */
            /* contribution for this (x,y) pair:
               for z=0 we have multiplicity ox*oy,
               for every z>0 we have multiplicity ox*oy*2  */
            inside += ox * oy * ((seq_t)2 * zcount - 1);
        }
    }

    seq_t result = total - inside;
    return result;
}

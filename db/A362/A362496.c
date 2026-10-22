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
 * Helpers:   antidiagonal_coords, newton_root
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
#include "../utils/antidiagonal_coords.h"
#include "../utils/newton_root.h"

seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);
    /* use swapped coordinates to match the OEIS convention */
    return newton_root(i, j);
}

/*
 * A288969  Triangular array read by rows: row n is the list of the 2*n-1
 *          successive values taken by the function z = n - floor(x) *
 *          floor(y) along the hyperbola with equation y = n/x, for 1 <= x <=
 *          n.
 * https://oeis.org/A288969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = value read from the triangular array where * row r
 *            contains values T(r,t) for t = -(r-1)..(r-1) with * T(r,t) = r -
 *            x*(x+t) where x = floor((-t + sqrt(t^2 + 4r))/2)
 * Range:     u8 n <= 36, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Find the row r that contains the index n.
       The first index of row r is (r-1)^2 + 1.
       Hence r = floor_sqrt(n-1) + 1, because integers start at 1. */
    idx_t r = (idx_t)floor_sqrt((seq_t)(n - 1)) + 1;

    /* Position of n within its row (1 .. 2r-1). */
    idx_t prev_sq = (r - 1) * (r - 1);
    idx_t pos = n - prev_sq;              /* 1‑based position in the row */

    /* Convert to signed column index t = pos - r. */
    long long t = (long long)pos - (long long)r;

    /* Compute sqrt(t^2 + 4*r). */
    long long tt = t * t;                 /* fits in 64 bits for n <= 10000 */
    seq_t sum = (seq_t)tt + (seq_t)4 * (seq_t)r;
    seq_t sqrt_val = floor_sqrt(sum);

    /* Compute x = floor((-t + sqrt_val)/2) safely using signed arithmetic. */
    long long x = ((long long)sqrt_val - t) / 2;

    /* Finally compute the required value. */
    long long xt = x * (x + t);
    seq_t result = (seq_t)r - (seq_t)xt;

    return result;
}

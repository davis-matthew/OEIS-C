/*
 * A387494  Array read by antidiagonals: T(n,k) is the maximum number of steps
 *          in a n^k cuboid with a rule that allows moving to neighboring
 *          visited cuboids if the next step is onto an unvisited cuboid when
 *          starting in a corner.
 * https://oeis.org/A387494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, more
 * Formula:   a(n) = T(i,j) read along antidiagonals. * For the index n
 *            (starting at 1), find d = smallest integer with * d*(d-1)/2 >=
 *            n. * Let pos = n - (d-2)*(d-1)/2 (1‑based position on the
 *            diagonal). * Then i = pos, j = d - pos. * T(n,k) for n≥1, k≥1 is
 *            given (conjecturally exact) by: * T(n,k) = 2*n^k - k*...
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   T_func, diag_d
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/T_func.h"
#include "../utils/diag_d.h"

/* Integer square root for unsigned __int128 (floor). */
static inline unsigned __int128 floor_sqrt128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = (((unsigned __int128)1) << 64);
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Main sequence function.  Decode (i,j) from the antidiagonal index n
 * and return T(i,j). */
seq_t a(idx_t n)
{
    /* Locate the diagonal d. */
    idx_t d = diag_d(n);

    /* Previous cumulative count before this diagonal: (d-2)*(d-1)/2. */
    unsigned __int128 prev =
        ((unsigned __int128)(d - 2) * (d - 1)) / 2;

    /* Position within the diagonal (1‑based). */
    idx_t pos = n - (idx_t)prev;

    /* Coordinates on the matrix. */
    idx_t i = pos;        /* row index */
    idx_t j = d - pos;    /* column index, decreases as i increases */

    return T_func(i, j);
}

/*
 * A321479  Regular triangle read by rows: T(n,k) is the period of
 *          {A316269(k,m)} modulo n, 0 <= k <= n - 1.
 * https://oeis.org/A321479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   period_lucas
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A321479
    a(n) = T(r,k) where
      r = smallest integer with r*(r+1)/2 >= n
      k = n - r*(r-1)/2 - 1            (0‑based column)
    T(r,k) is the period of the Lucas sequence
      u0 = 0 , u1 = 1
      u_{m+1} = k*u_m – u_{m-1} (mod r)
    The period is the smallest l>0 such that u_l = 0 and u_{l+1} = 1.
    (The period is at most 3*r.)  */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/period_lucas.h"

seq_t a(idx_t n) {
    /* find row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((r * (r + 1) / 2) < n)
        ++r;

    /* column k (0‑based) inside that row */
    idx_t k = n - ((r * (r - 1) / 2)) - 1;

    /* return period of Lucas sequence modulo r */
    if (r == 1)          /* row 1 contains only the value 1 */
        return 1;
    return period_lucas(r, k);
}

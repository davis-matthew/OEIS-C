/*
 * A309853  Array read by antidiagonals: ((z+sqrt(x))/2)^k + ((z-sqrt(x))/2)^k
 *          for columns k >= 0 and rows n >= 0, where x = 4*n+1 and y =
 *          ceiling(sqrt(x)) and z = y+1-(y mod 2).
 * https://oeis.org/A309853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A309853.
   The array entry a(row,k) is defined by
        a(row,k) = ((z+√x)/2)^k + ((z-√x)/2)^k
   where x = 4*row + 1,
         y = ceil(√x),
         z = y + 1 - (y mod 2).
   The per‑row sequence satisfies the recurrence
        a_0 = 2
        a_1 = z
        a_{k+2} = z*a_{k+1} - d*a_k
   with d = (z*z - x)/4.
   The overall sequence a(n) is obtained by reading this array
   along antidiagonals: for each s = row+col the pairs
   (row,col) with row+col = s are taken in increasing row order.
   a(0) is (0,0), a(1) is (0,1), a(2) is (1,0), etc. */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Determine the antidiagonal index s and the positions (row,k) */
    idx_t s = 0;
    while (((s + 1) * (s + 2)) / 2 <= n)
        ++s;
    idx_t count_before = (s * (s + 1)) / 2;   /* total elements before this anti‑diagonal */
    idx_t t = n - count_before;               /* offset within this diagonal (row index) */
    idx_t row = t;                            /* n-coordinate of the entry */
    idx_t col = s - t;                        /* k-coordinate of the entry */

    /* Compute x = 4*row + 1  (as seq_t) */
    seq_t x = (seq_t)4 * row + 1;

    /* Compute y = ceil(sqrt(x)) */
    seq_t floorSq = floor_sqrt(x);
    seq_t y = (floorSq * floorSq == x) ? floorSq : floorSq + 1;

    /* Compute z = y + 1 - (y mod 2) */
    seq_t z = y + 1 - (y % 2);

    /* Base cases */
    if (col == 0) return (seq_t)2;
    if (col == 1) return z;

    /* Recurrence: a_{k+2} = z*a_{k+1} - d*a_k ,  d = (z*z - x)/4 */
    seq_t d = (z * z - x) / 4;
    seq_t a_prev2 = (seq_t)2;   /* a_0 */
    seq_t a_prev1 = z;          /* a_1 */
    seq_t a_curr;

    for (idx_t i = 2; i <= col; ++i) {
        a_curr = z * a_prev1 - d * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_prev1;
}

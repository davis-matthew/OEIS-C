/*
 * A325027  Triangle read by rows: T(n,k), 0 <= k < n, is the least m
 *          minimizing the number of intervals [a,a+1) or [ma,m(a+1)) that
 *          must be XORed together to form the interval [k,n).
 * https://oeis.org/A325027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(row, col) where T(row,col) is the least m minimizing
 *            F(row,col,m); indices start at 1
 * Range:     u8 n <= 2480, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   triangle_min_m
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard includes */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/triangle_min_m.h"

/* Main sequence function: returns the nth term (offset 1) */
seq_t a(idx_t n)
{
    /* Determine row and column from the serial index n */
    idx_t row = 1;
    while (row * (row + 1) / 2 < n)
        ++row;

    idx_t prev_rows = (row - 1) * row / 2;
    idx_t k = n - prev_rows - 1;   /* 0‑based column */

    return triangle_min_m(row, k);
}

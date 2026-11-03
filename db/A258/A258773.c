/*
 * A258773  Triangle read by rows, T(n,k) = (-1)^(n-k)*C(n,k)*k^n, for n >= 0
 *          and 0 <= k <= n.
 * https://oeis.org/A258773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 24, i32 n <= 59, i64 n <= 144,
 *            i128 n <= 367, big any n
 * Validated: standard, pass, 61 terms through n = 1299 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the OEIS triangle A258773 read by rows.
   T(n,k) = (-1)^(n-k) * C(n,k) * k^n,  for n >= 0, 0 <= k <= n.
   The function returns the (row‑major) flattening of this triangle:
   a(0) = T(0,0)    = 1
   a(1) = T(1,0)    = 0
   a(2) = T(1,1)    = 1
   ...
*/
#include <stdbool.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* Find the row such that row*(row+1)/2 <= n < (row+1)*(row+2)/2  */
    idx_t row = 0;
    while ((row + 1) * (row + 2) / 2 <= n)
        ++row;

    /* Column index within the row */
    idx_t start = row * (row + 1) / 2;
    idx_t col   = n - start;

    /* Handle the only special case where 0^0 appears.  */
    if (row == 0 && col == 0)
        return (seq_t)1;

    /* Compute T(row,col) = (-1)^(row-col) * C(row,col) * col^row  */
    seq_t bin  = binomial(row, col);                     /* C(row,col)   */
    seq_t powv = ipow((seq_t)col, (seq_t)row);     /* col^row      */
    seq_t prod = bin * powv;                          /* product      */

    if (((row - col) & 1u) != 0u)   /* apply the sign */
        prod = -prod;

    return prod;
}

/*
 * A299741  Array read by antidiagonals upwards: a(i,0) = 2, i >= 0; a(i,1) =
 *          i+2, i >= 0; a(i,j) = (i+2) * a(i,j-1) - a(i,j-2), for i >= 0, j >
 *          1.
 * https://oeis.org/A299741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   a_at
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A299741
   The array a(i,j) is defined by
       a(i,0) = 2,
       a(i,1) = i+2,
       a(i,j) = (i+2)*a(i,j-1) - a(i,j-2)   for j > 1.
   The sequence a(n) is obtained by reading this array by antidiagonals
   upwards: for each k = i+j, the pairs (k,0), (k-1,1), … ,(0,k) are
   visited in that order.
   a(n) is the a(i,j) that lies on the nth entry in that order. */

#include <stdint.h>
#include "../utils/a_at.h"

/* Return the diagonal number k such that k*(k+1)/2 <= n < (k+1)*(k+2)/2. */
static inline idx_t diag_of(idx_t n)
{
    idx_t k = 0;
    idx_t cum = 1;                /* number of terms up to diagonal 0 */
    while (cum <= n) {
        ++k;                      /* move to next diagonal */
        cum += k + 1;             /* this diagonal has k+1 elements */
    }
    return k;
}

seq_t a(idx_t n)
{
    idx_t k = diag_of(n);
    idx_t tri = k * (k + 1) / 2;            /* terms before diagonal k */
    idx_t offset = n - tri;                 /* offset within diagonal */
    idx_t i = k - offset;                  /* row index */
    idx_t j = offset;                      /* column index */
    return a_at(i, j);
}

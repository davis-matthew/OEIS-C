/*
 * A365644  Array read by ascending antidiagonals: A(n, k) = k*(10^n - 1)/9
 *          with k >= 0.
 * https://oeis.org/A365644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, tabl
 * Formula:   a(n) = value of the array A(i,j) = j*(10^i-1)/9 read by
 *            ascending antidiagonals
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Find the diagonal index d that contains the term a(n). 
       The cumulative number of terms up to diagonal d is 
       sum_{k=0}^{d} (k+1) = (d+1)(d+2)/2. */

    idx_t d = 0;
    idx_t cumulative = 0;           /* terms before the current diagonal */

    while (cumulative + (d + 1) <= n) {
        cumulative += (d + 1);
        ++d;
    }

    idx_t j = n - cumulative;        /* column index (k) within the diagonal */
    idx_t r = d - j;                  /* row index (i) within the diagonal */

    /* Compute A(i,j) = j * (10^i - 1) / 9  */
    seq_t val = pow10(r);             /* 10^i */
    val = val - (seq_t)1;             /* 10^i - 1 */
    val = val / (seq_t)9;             /* (10^i - 1) / 9 */
    val = val * (seq_t)j;             /* * j */

    return val;
}

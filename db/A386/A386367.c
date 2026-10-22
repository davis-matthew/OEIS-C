/*
 * A386367  a(n) = Sum_{k=0..n-1} binomial(5*k,k) * binomial(5*n-5*k-2,n-k-1).
 * https://oeis.org/A386367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \displaystyle\sum_{k=0}^{n-1} 4^{\,n-k-1}\binom{5n-1}{k}
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    /* 4^(n-1)  (can be zero for big n if seq_t overflows – fits the type) */
    seq_t pow4 = pow_int(4, n - 1);

    seq_t sum  = 0;
    seq_t binom = 1;              /* C(5n-1,0) */
    idx_t limit = n;              /* upper bound for k   */

    for (idx_t k = 0; k < limit; ++k) {
        sum += pow4 * binom;
        if (k + 1 < limit) {
            /* C(5n-1,k+1) = C(5n-1,k) * (5n-1-k)/(k+1)  */
            binom = binom * ((5 * n - 1) - k) / (k + 1);
            /* 4^(n-k-2) from previous pow4  */
            pow4 = pow4 / 4;
        }
    }

    return sum;
}

/*
 * A262258  a(n) = the number of hills (arch length of 1 with no covering
 *          arches) for semi-meander solutions with n arches and
 *          floor((n+2)/2) arch group returns to the x axis.
 * https://oeis.org/A262258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, for n>=3: a(n)=2^(floor((n-1)/2)-1) *
 *            (2*floor(n/2)-floor((n-3)/2))
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (same as a(n)=2^floor((n-1)/2)*(floor(n/2)-floor((n-3)/2)/2)) */
#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n < 2) return 1;          /* a(1)=1, and for safety a(0)=1 (undefined) */
    if (n == 2) return 2;         /* a(2)=2 */

    /* n >= 3 */
    idx_t m   = (n - 1) / 2;                 /* floor((n-1)/2)  */
    idx_t t1  = n / 2;                       /* floor(n/2)      */
    idx_t t2  = (n - 3) / 2;                 /* floor((n-3)/2)  */

    seq_t inner = (seq_t)(2 * t1 - t2);      /* (2*floor(n/2) - floor((n-3)/2)) */
    seq_t pow_val = pow2(m - 1);             /* 2^(floor((n-1)/2)-1)   */
    return pow_val * inner;
}

/*
 * A250774  Number of (6+1) X (n+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 28*2^(n-1) + 26*n^2 + 120*n + 114 (n >= 1)
 * Range:     u8 none, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* index below offset: undefined, return 0 */

    seq_t nn = (seq_t)n;                      /* convert index to seq_t */
    seq_t p2 = pow2(n - 1);                  /* 2^(n-1) */
    seq_t res = (seq_t)28 * p2;               /* 28*2^(n-1) */
    res += (seq_t)26 * nn * nn;               /* +26*n^2 */
    res += (seq_t)120 * nn;                   /* +120*n */
    res += (seq_t)114;                        /* +114 */
    return res;
}

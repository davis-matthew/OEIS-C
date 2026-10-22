/*
 * A318126  a(n) is the number of pieces of the simplest continuous piecewise
 *          linear function that agrees with n mod k for all positive integer
 *          k.
 * https://oeis.org/A318126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) – number of intervals needed to model n mod k
   via the least number of linear pieces with integer endpoints.
   Computation: build the sequence of n mod k for k = 1..n+1,
   count the changes of the difference between successive terms
   (the slope).  Each change starts a new interval.
   The final constant tail (k > n+1) is an additional
   interval unless the last slope already equals 0. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* all values are 0; one constant interval */
        return 1;

    seq_t prev_mod   = (seq_t)n % (seq_t)1;   /* n mod 1 == 0 */
    seq_t prev_slope = 0;                     /* dummy, overwritten at k==2 */
    seq_t groups     = 0;
    seq_t slope = 0;                           /* to keep last slope after loop */

    for (idx_t k = 2; k <= n + 1; ++k) {
        seq_t cur_mod = (seq_t)n % (seq_t)k;
        slope = cur_mod - prev_mod;            /* unsigned diff works for sign */

        if (k == 2) {
            groups = 1;                        /* first interval starts at k=1 */
        } else if (slope != prev_slope) {
            ++groups;                          /* slope change → new interval */
        }

        prev_mod = cur_mod;
        prev_slope = slope;
    }

    /* add the final constant tail unless last slope was already 0 */
    return groups + (slope != 0);
}

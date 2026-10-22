/*
 * A385071  a(n) = number of triangular numbers <= n-th tetrahedral number.
 * https://oeis.org/A385071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((3 + sqrt(9 + 12*n*(n+1)*(n+2)))/6)
 * Range:     u8 n <= 0, u16 n <= 10, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;                 /* cast index to seq_t */
    seq_t prod = nn * (nn + 1) * (nn + 2); /* n*(n+1)*(n+2)              */
    seq_t u    = 9 + (prod * 12);          /* 9 + 12*n*(n+1)*(n+2)       */
    seq_t s    = floor_sqrt(u);            /* floor(sqrt(u))              */
    return (3 + s) / 6;                    /* floor((3+sqrt(u))/6)       */
}

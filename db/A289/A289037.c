/*
 * A289037  Positions of 1 in A289035; complement of A289036.
 * https://oeis.org/A289037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(2+sqrt(2))) - [ {n*sqrt(2)} < sqrt(2)-1 ]
 * Range:     u8 n <= 5, u16 n <= 58, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = 2*n + floor(n*sqrt(2)) - ε where ε = 1 iff 2*(n-1)^2 < (floor(n*sqrt(2))-1)^2 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;                           /* cast n to seq_t */
    seq_t two_n2 = 2 * sn * sn;                    /* 2 * n^2   */
    seq_t k = floor_sqrt(two_n2);                  /* floor(n*sqrt(2)) */
    seq_t m = 2 * sn + k;                          /* floor(n*(2+sqrt(2))) */

    /* Determine whether to subtract 1 */
    seq_t sub = 0;
    seq_t n1 = sn - 1;                             /* n-1       */
    seq_t lhs = 2 * n1 * n1;                       /* 2*(n-1)^2 */
    seq_t km1 = k - 1;                             /* k-1       */
    seq_t rhs = km1 * km1;                         /* (k-1)^2   */
    if (lhs < rhs)
        sub = 1;

    return m - sub;
}

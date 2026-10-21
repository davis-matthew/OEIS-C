/*
 * A260734  a(n) = number of steps needed to reach (n^2)-1 when starting from
 *          k = ((n+1)^2)-1 and repeatedly applying the map that replaces k
 *          with k - A002828(k), where A002828(k) = the least number of
 *          squares that add up to k.
 * https://oeis.org/A260734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps needed to reach (n^2)-1 from ((n+1)^2)-1
 *            * under the map k -> k - A002828(k), where A002828(k) is the
 *            minimal number * of integer squares summing to k
 * Range:     u8 n <= 7, u16 n <= 95, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   can_two_squares, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"
#include "../utils/can_two_squares.h"

/* minimal number of squares that represent n (1 ≤ result ≤ 4).  n > 0. */
static seq_t min_squares(seq_t n)
{
    seq_t sq = floor_sqrt(n);
    if (sq * sq == n)
        return 1;

    uint64_t nn = (uint64_t)n;
    if (can_two_squares(nn))
        return 2;

    /* check if n has the form 4^a(8b+7), which guarantees 4 squares */
    uint64_t t = nn;
    while ((t & 3) == 0)      /* divide by 4 */
        t >>= 2;
    if ((t & 7) == 7)         /* remainder 7 modulo 8 */
        return 4;

    return 3;                 /* otherwise 3 squares suffice */
}

seq_t a(idx_t n)
{
    /* start from ((n+1)^2)-1, end at (n^2)-1 */
    seq_t nn      = (seq_t)(n + 1);
    seq_t start   = nn * nn - 1;
    seq_t target  = (seq_t)n * n - 1;

    seq_t count = 0;
    while (start > target) {
        seq_t step = min_squares(start);
        start -= step;
        ++count;
    }
    return count;
}
